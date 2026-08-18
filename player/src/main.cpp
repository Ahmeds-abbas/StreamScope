#include <gst/gst.h>
#include <iostream>
#include <string>
#include "streamscope/buffer_model.hpp"
#include "streamscope/hls_manifest.hpp"
#include "streamscope/playback_state.hpp"
#include "streamscope/segment_scheduler.hpp"
#include "streamscope/http_downloader.hpp"
#include <gst/app/gstappsrc.h>

int main(int argc, char* argv[])
{
    int exitCode = 0;
    gst_init(&argc, &argv);

    if (argc < 2)
    {
        std::cerr << "Usage: streamscope_player <HLS URL>\n";
        return 1;
    }

    const std::string streamUrl = argv[1];

    const auto representations =
        parseMasterPlaylist("assets/generated/master.m3u8");

    if (representations.empty())
    {
        std::cerr << "No representations found.\n";
        return 1;
    }

    const Representation& selectedRepresentation =
        representations.front();

    const std::size_t slashPosition =
        selectedRepresentation.playlistUrl.find_last_of('/');

    const std::string representationDirectory =
        selectedRepresentation.playlistUrl.substr(
            0,
            slashPosition + 1
        );

    const std::string mediaPlaylistPath =
        "assets/generated/" + selectedRepresentation.playlistUrl;

    const auto segments =
        parseMediaPlaylist(mediaPlaylistPath);

    GError* pipelineError = nullptr;

    GstElement* pipeline = gst_parse_launch(
        "appsrc name=source ! "
        "tsdemux name=demux "
        "demux. ! queue ! h264parse ! avdec_h264 ! videoconvert ! autovideosink "
        "demux. ! queue ! aacparse ! avdec_aac ! audioconvert ! audioresample ! autoaudiosink",
        &pipelineError
    );

    if (pipeline == nullptr)
    {
        std::cerr << "Failed to create appsrc pipeline.\n";

        if (pipelineError != nullptr)
        {
            std::cerr << pipelineError->message << '\n';
            g_clear_error(&pipelineError);
        }

        return 1;
    }

    GstElement* source =
        gst_bin_get_by_name(
            GST_BIN(pipeline),
            "source"
        );

    if (source == nullptr)
    {
        std::cerr << "Could not find appsrc element.\n";
        gst_object_unref(pipeline);
        return 1;
    }

    GstCaps* caps = gst_caps_from_string(
        "video/mpegts, systemstream=(boolean)true, packetsize=(int)188"
    );

    gst_app_src_set_caps(
        GST_APP_SRC(source),
        caps
    );

    gst_caps_unref(caps);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    const GstStateChangeReturn stateResult =
        gst_element_get_state(
            pipeline,
            nullptr,
            nullptr,
            5 * GST_SECOND
        );

    std::cout << "Pipeline state result: "
              << stateResult
              << '\n';

    SegmentScheduler scheduler(segments);
    BufferModel buffer;
    PlaybackStateMachine stateMachine;

    stateMachine.transitionTo(PlaybackState::Buffering);

    while (const Segment* segment = scheduler.next())
    {
        const std::string segmentUrl =
            "http://127.0.0.1:8000/" +
            representationDirectory +
            segment->url;

        const DownloadResult result = downloadUrl(segmentUrl);

        if (!result.success)
        {
            std::cerr << "Failed to download segment "
                      << segment->sequence << '\n';
            break;
        }

        GstBuffer* gstBuffer =
            gst_buffer_new_allocate(
                nullptr,
                result.data.size(),
                nullptr
            );

        gst_buffer_fill(
            gstBuffer,
            0,
            result.data.data(),
            result.data.size()
        );

        const GstFlowReturn pushResult =
            gst_app_src_push_buffer(
                GST_APP_SRC(source),
                gstBuffer
            );

        if (pushResult != GST_FLOW_OK)
        {
            std::cerr << "Failed to push segment "
                      << segment->sequence << '\n';
            break;
        }

        buffer.add(segment->duration);

        std::cout << "Buffer level: "
                  << buffer.level()
                  << " seconds\n";

        std::cout << "Pushed segment "
                  << segment->sequence << '\n';
    }

    gst_app_src_end_of_stream(
        GST_APP_SRC(source)
    );

    GstBus* appBus = gst_element_get_bus(pipeline);

    gint64 previousPosition = 0;
    GstMessage* appMessage = nullptr;

    while (true)
    {
        appMessage =
            gst_bus_timed_pop_filtered(
                appBus,
                250 * GST_MSECOND,
                static_cast<GstMessageType>(
                    GST_MESSAGE_ERROR | GST_MESSAGE_EOS
                )
            );

        gint64 currentPosition = 0;

        if (gst_element_query_position(
                pipeline,
                GST_FORMAT_TIME,
                &currentPosition))
        {
            const double deltaSeconds =
                static_cast<double>(
                    currentPosition - previousPosition
                ) / GST_SECOND;

            if (deltaSeconds > 0.0)
            {
                buffer.consume(deltaSeconds);

                std::cout << "Buffer level: "
                          << buffer.level()
                          << " seconds\n";
            }

            if (currentPosition > 0 &&
                stateMachine.current() == PlaybackState::Buffering)
            {
                stateMachine.transitionTo(PlaybackState::Playing);

                std::cout << "State: PLAYING\n";
            }

            previousPosition = currentPosition;
        }

        if (appMessage != nullptr)
        {
            if (GST_MESSAGE_TYPE(appMessage) == GST_MESSAGE_EOS)
            {
                stateMachine.transitionTo(PlaybackState::Ended);
                std::cout << "State: ENDED\n";
            }
            else if (GST_MESSAGE_TYPE(appMessage) == GST_MESSAGE_ERROR)
            {
                stateMachine.transitionTo(PlaybackState::Error);
                std::cout << "State: ERROR\n";
            }

            break;
        }
    }

    if (GST_MESSAGE_TYPE(appMessage) == GST_MESSAGE_EOS)
    {
        std::cout << "appsrc test finished successfully.\n";
    }
    else if (GST_MESSAGE_TYPE(appMessage) == GST_MESSAGE_ERROR)
    {
        GError* error = nullptr;
        gchar* debugInfo = nullptr;

        gst_message_parse_error(
            appMessage,
            &error,
            &debugInfo
        );

        std::cerr << "appsrc playback failed: "
                  << error->message << '\n';

        g_clear_error(&error);
        g_free(debugInfo);
    }

    gst_message_unref(appMessage);

    gst_object_unref(appBus);

    gst_element_set_state(
        pipeline,
        GST_STATE_NULL
    );

    gst_object_unref(source);
    gst_object_unref(pipeline);

    return 0;

    GstElement* player =
        gst_element_factory_make("playbin", "streamscope-player");

    if (player == nullptr)
    {
        std::cerr << "Failed to create GStreamer playbin.\n";
        return 1;
    }
    g_object_set(player, "uri", streamUrl.c_str(), nullptr);

    std::cout << "Stream URL: " << streamUrl << '\n';
    const GstStateChangeReturn playerStateResult =
        gst_element_set_state(player, GST_STATE_PLAYING);

    if (playerStateResult == GST_STATE_CHANGE_FAILURE)
    {
        std::cerr << "Failed to start playback.\n";
        gst_object_unref(player);
        return 1;
    }
    GstBus* bus = gst_element_get_bus(player);

    GstMessage* message = gst_bus_timed_pop_filtered(
        bus,
        GST_CLOCK_TIME_NONE,
        static_cast<GstMessageType>(
            GST_MESSAGE_ERROR | GST_MESSAGE_EOS
        )
    );

    if (message != nullptr)
    {
        switch (GST_MESSAGE_TYPE(message))
        {
            case GST_MESSAGE_EOS:
                std::cout << "Playback finished successfully.\n";
                break;

            case GST_MESSAGE_ERROR:
            {
                GError* error = nullptr;
                gchar* debugInfo = nullptr;

                gst_message_parse_error(
                    message,
                    &error,
                    &debugInfo
                );

                std::cerr << "Playback failed: "
                          << error->message
                          << '\n';

                if (debugInfo != nullptr)
                {
                    std::cerr << "Debug information: "
                              << debugInfo
                              << '\n';
                }

                g_clear_error(&error);
                g_free(debugInfo);
                exitCode = 1;

                break;
            }

            default:
                break;
        }

        gst_message_unref(message);
    }

    gst_object_unref(bus);

    gst_element_set_state(
        player,
        GST_STATE_NULL
    );

    gst_object_unref(player);

    return exitCode;
}

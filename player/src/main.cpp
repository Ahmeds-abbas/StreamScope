#include <gst/gst.h>
#include <iostream>
#include <string>
#include "streamscope/hls_manifest.hpp"
#include "streamscope/segment_scheduler.hpp"
#include "streamscope/http_downloader.hpp"

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

SegmentScheduler scheduler(segments);

while (const Segment* segment = scheduler.next())
{
    const std::string segmentUrl =
        "http://127.0.0.1:8000/" +
        representationDirectory +
        segment->url;

    const DownloadResult result =
        downloadUrl(segmentUrl);

    std::cout
        << "Segment " << segment->sequence
        << " | HTTP: " << result.httpStatus
        << " | bytes: " << result.data.size()
        << " | time: " << result.durationSeconds
        << " | success: " << result.success
        << '\n';

    if (!result.success)
    {
        std::cerr
            << "Failed to download segment "
            << segment->sequence
            << '\n';

        return 1;
    }
}

    GstElement* player =
        gst_element_factory_make("playbin", "streamscope-player");

    if (player == nullptr)
    {
        std::cerr << "Failed to create GStreamer playbin.\n";
        return 1;
    }
    g_object_set(player, "uri", streamUrl.c_str(), nullptr);

    std::cout << "Stream URL: " << streamUrl << '\n';
    const GstStateChangeReturn stateResult =
        gst_element_set_state(player, GST_STATE_PLAYING);

    if (stateResult == GST_STATE_CHANGE_FAILURE)
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

///does this work??

#include <gst/gst.h>
#include <iostream>
#include <string>
#include "streamscope/hls_manifest.hpp"

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
    for (const Representation& rep : representations)
    {
    std::cout
        << rep.width << "x" << rep.height
        << " | bandwidth: " << rep.bandwidth
        << " | playlist: " << rep.playlistUrl
        << '\n';
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

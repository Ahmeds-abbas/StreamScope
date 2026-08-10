#include "streamscope/hls_manifest.hpp"

#include <fstream>
#include <iostream>
#include <string>

std::vector<Representation> parseMasterPlaylist(
    const std::string& filePath
)
{
    std::vector<Representation> representations;

    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cerr << "Could not open manifest: "
                  << filePath
                  << '\n';

        return representations;
    }

    std::string line;

    while (std::getline(file, line))
    {
        if (line.find("#EXT-X-STREAM-INF:") != std::string::npos)
        {
            int bandwidth = 0;
            int width = 0;
            int height = 0;

            // Parse BANDWIDTH
            const std::string bandwidthKey = "BANDWIDTH=";
            const std::size_t bandwidthStart =
                line.find(bandwidthKey);

            if (bandwidthStart != std::string::npos)
            {
                const std::size_t valueStart =
                    bandwidthStart + bandwidthKey.length();

                const std::size_t valueEnd =
                    line.find(',', valueStart);

                const std::string bandwidthText =
                    line.substr(
                        valueStart,
                        valueEnd - valueStart
                    );

                bandwidth = std::stoi(bandwidthText);
            }

            // Parse RESOLUTION
            const std::string resolutionKey = "RESOLUTION=";
            const std::size_t resolutionStart =
                line.find(resolutionKey);

            if (resolutionStart != std::string::npos)
            {
                const std::size_t valueStart =
                    resolutionStart + resolutionKey.length();

                const std::size_t xPosition =
                    line.find('x', valueStart);

                const std::size_t valueEnd =
                    line.find(',', xPosition);

                const std::string widthText =
                    line.substr(
                        valueStart,
                        xPosition - valueStart
                    );

                const std::string heightText =
                    line.substr(
                        xPosition + 1,
                        valueEnd - xPosition - 1
                    );

                width = std::stoi(widthText);
                height = std::stoi(heightText);
            }

            // The playlist URL is the line after EXT-X-STREAM-INF
            std::string playlistUrl;

            if (std::getline(file, playlistUrl))
            {
                Representation rep;

                rep.bandwidth = bandwidth;
                rep.width = width;
                rep.height = height;
                rep.playlistUrl = playlistUrl;

                representations.push_back(rep);
            }
        }
    }

    return representations;
}

std::vector<Segment> parseMediaPlaylist(
    const std::string& filePath
)
{
    std::vector<Segment> segments;

    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cerr << "Could not open media playlist: "
                  << filePath
                  << '\n';

        return segments;
    }

    std::string line;
    int sequence = 0;

    while (std::getline(file, line))
    {
        if (line.starts_with("#EXTINF:"))
        {
            const std::size_t colon = line.find(':');
const std::size_t comma = line.find(',');

const std::string durationText =
    line.substr(colon + 1, comma - colon - 1);

const double duration = std::stod(durationText);

std::string segmentUrl;

if (std::getline(file, segmentUrl))
{
    Segment segment;

    segment.sequence = sequence;
    segment.duration = duration;
    segment.url = segmentUrl;

    segments.push_back(segment);

    sequence++;
}
        }
    }

    return segments;
}

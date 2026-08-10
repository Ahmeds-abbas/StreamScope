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
            const std::string key = "BANDWIDTH=";
const std::size_t start = line.find(key);

if (start != std::string::npos)
{
    const std::size_t valueStart = start + key.length();
    const std::size_t valueEnd = line.find(',', valueStart);

    const std::string bandwidthText =
        line.substr(valueStart, valueEnd - valueStart);

    const int bandwidth = std::stoi(bandwidthText);

    std::cout << "Parsed bandwidth: "
              << bandwidth
              << '\n';
}
            std::cout << "Found representation line:\n"
                      << line
                      << '\n';
        const std::string resolutionKey = "RESOLUTION=";
const std::size_t resolutionStart = line.find(resolutionKey);

if (resolutionStart != std::string::npos)
{
    const std::size_t valueStart =
        resolutionStart + resolutionKey.length();

    const std::size_t xPosition =
        line.find('x', valueStart);

    const std::size_t valueEnd =
        line.find(',', xPosition);

    const std::string widthText =
        line.substr(valueStart, xPosition - valueStart);

    const std::string heightText =
        line.substr(xPosition + 1, valueEnd - xPosition - 1);

    const int width = std::stoi(widthText);
    const int height = std::stoi(heightText);

    std::cout << "Parsed resolution: "
              << width << "x" << height << '\n';
}

                    }
    }

    return representations;
}
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

    return representations;
}
std::string line;

while (std::getline(file, line))
{
    std::cout << line << '\n';
}
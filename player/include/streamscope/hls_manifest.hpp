#pragma once

#include <string>
#include <vector>

struct Representation
{
    std::string playlistUrl;
    int bandwidth;
    int width;
    int height;
};

std::vector<Representation> parseMasterPlaylist(
    const std::string& filePath
);
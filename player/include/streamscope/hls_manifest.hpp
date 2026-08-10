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

struct Segment
{
    int sequence;
    double duration;
    std::string url;
};
std::vector<Representation> parseMasterPlaylist(
    const std::string& filePath
);
std::vector<Segment> parseMediaPlaylist(
    const std::string& filePath
);
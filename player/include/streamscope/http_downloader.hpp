#pragma once

#include <string>
#include <vector>

struct DownloadResult
{
    bool success = false;
    long httpStatus = 0;
    double durationSeconds = 0.0;

    std::vector<unsigned char> data;
    std::string errorMessage;
};

DownloadResult downloadUrl(
    const std::string& url
);

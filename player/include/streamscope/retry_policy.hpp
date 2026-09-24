#pragma once

#include "streamscope/http_downloader.hpp"

bool isRetryableDownloadFailure(
    const DownloadResult& result
);

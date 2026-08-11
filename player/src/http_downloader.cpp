#include "streamscope/http_downloader.hpp"

#include <curl/curl.h>
#include <vector>

static size_t writeCallback(
    char* ptr,
    size_t size,
    size_t nmemb,
    void* userdata
)
{
    const size_t totalBytes = size * nmemb;

    auto* data =
        static_cast<std::vector<unsigned char>*>(userdata);

    const auto* begin =
        reinterpret_cast<unsigned char*>(ptr);

    data->insert(
        data->end(),
        begin,
        begin + totalBytes
    );

    return totalBytes;
}

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

DownloadResult downloadUrl(
    const std::string& url
)
{
    DownloadResult result;

    CURL* curl = curl_easy_init();

    if (curl == nullptr)
    {
        result.errorMessage = "Failed to create libcurl handle.";
        return result;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        writeCallback
    );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEDATA,
        &result.data
    );

    const CURLcode curlResult = curl_easy_perform(curl);

    if (curlResult != CURLE_OK)
    {
        result.errorMessage = curl_easy_strerror(curlResult);
        curl_easy_cleanup(curl);
        return result;
    }

    curl_easy_getinfo(
        curl,
        CURLINFO_RESPONSE_CODE,
        &result.httpStatus
    );

    curl_easy_getinfo(
        curl,
        CURLINFO_TOTAL_TIME,
        &result.durationSeconds
    );

    result.success =
        result.httpStatus >= 200 &&
        result.httpStatus < 300;

    curl_easy_cleanup(curl);

    return result;
}

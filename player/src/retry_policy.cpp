#include "streamscope/retry_policy.hpp"

bool isRetryableDownloadFailure(
    const DownloadResult& result
)
{
    if (result.success)
    {
        return false;
    }

    if (result.httpStatus == 0)
    {
        return true;
    }

    return result.httpStatus == 408 ||
           result.httpStatus == 429 ||
           (result.httpStatus >= 500 && result.httpStatus <= 599);
}

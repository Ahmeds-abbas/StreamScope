#include <gtest/gtest.h>

#include "streamscope/retry_policy.hpp"

TEST(RetryPolicyTest, DoesNotRetryNotFound)
{
    DownloadResult result;
    result.httpStatus = 404;

    EXPECT_FALSE(isRetryableDownloadFailure(result));
}

TEST(RetryPolicyTest, RetriesInternalServerError)
{
    DownloadResult result;
    result.httpStatus = 500;

    EXPECT_TRUE(isRetryableDownloadFailure(result));
}

TEST(RetryPolicyTest, RetriesServiceUnavailable)
{
    DownloadResult result;
    result.httpStatus = 503;

    EXPECT_TRUE(isRetryableDownloadFailure(result));
}

TEST(RetryPolicyTest, RetriesRequestTimeout)
{
    DownloadResult result;
    result.httpStatus = 408;

    EXPECT_TRUE(isRetryableDownloadFailure(result));
}

TEST(RetryPolicyTest, RetriesTooManyRequests)
{
    DownloadResult result;
    result.httpStatus = 429;

    EXPECT_TRUE(isRetryableDownloadFailure(result));
}

TEST(RetryPolicyTest, RetriesNetworkFailure)
{
    DownloadResult result;
    result.errorMessage = "Connection failed";

    EXPECT_TRUE(isRetryableDownloadFailure(result));
}

TEST(RetryPolicyTest, DoesNotRetrySuccessfulRequest)
{
    DownloadResult result;
    result.success = true;
    result.httpStatus = 200;

    EXPECT_FALSE(isRetryableDownloadFailure(result));
}

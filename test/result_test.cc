#include "gtest/gtest.h"
#include "snapshot/snapshot.h"

#include "./custom_result.h"

#include "result/result.h"
#include "result/result_or.h"
#include "result/types_check/has_error_code_to_str.h"

using namespace custom_result;

class ResultTest : public testing::Test {
protected:
    virtual void SetUp() override {}
};

TEST_F(ResultTest, has_error_code_to_str_test) {
    EXPECT_TRUE(result::internal::has_error_code_to_str_v<ErrorCode>);
}

TEST_F(ResultTest, result_test) {
    {
        auto res = CustomResult::Builder(CustomResult::ErrorCode::kOK).Build();
        auto error_message = res.Message();
        EXPECT_TRUE(res.IsOK());
        EXPECT_EQ(error_message, std::string("OK"));
    }

    {
        auto res = CustomResult::Builder(CustomResult::ErrorCode::kOtherError).Build();
        auto error_message = res.Message();
        EXPECT_TRUE(res.Is(CustomResult::ErrorCode::kOtherError));
        EXPECT_FALSE(res.IsOK());
        EXPECT_EQ(error_message, std::string("OtherError"));
    }

    {
        auto res = CustomResult::Builder(CustomResult::ErrorCode::kOtherError).WithErrorMessage("ddd").Build();
        auto error_message = res.Message();
        EXPECT_TRUE(res.Is(CustomResult::ErrorCode::kOtherError));
        EXPECT_FALSE(res.IsOK());
        EXPECT_EQ(error_message, std::string("ddd"));
    }
}

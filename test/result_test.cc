#include "gtest/gtest.h"
#include "result/macros.h"
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
        auto res = CustomResult::Builder(CustomResult::ErrorCode::OK).Build();
        auto error_message = res.Message();
        EXPECT_TRUE(res.IsOK());
        EXPECT_EQ(error_message, std::string("OK"));
    }

    {
        auto res = CustomResult::Builder(CustomResult::ErrorCode::OtherError).Build();
        auto error_message = res.Message();
        EXPECT_TRUE(res.Is(CustomResult::ErrorCode::OtherError));
        EXPECT_FALSE(res.IsOK());
        EXPECT_EQ(error_message, std::string("OtherError"));
    }

    {
        auto res = CustomResult::Builder(CustomResult::ErrorCode::OtherError).WithErrorMessage("ddd").Build();
        auto error_message = res.Message();
        EXPECT_TRUE(res.Is(CustomResult::ErrorCode::OtherError));
        EXPECT_FALSE(res.IsOK());
        EXPECT_EQ(error_message, std::string("ddd"));
    }

    {
        EXPECT_EQ("OK", CustomResult::ErrorCodeToStr(CustomResult::ErrorCode::OK));
        EXPECT_EQ("OtherError", CustomResult::ErrorCodeToStr(CustomResult::ErrorCode::OtherError));
    }
}

TEST_F(ResultTest, result_or_test) {
    {
        auto res = CustomResultOr<int>::Builder(CustomResult::ErrorCode::OK).Build();
        auto error_message = res.Message();
        EXPECT_TRUE(res.IsOK());
        EXPECT_EQ(error_message, std::string("OK"));
    }

    {
        auto res = CustomResultOr<int>::Builder(CustomResult::ErrorCode::OtherError).Build();
        auto error_message = res.Message();
        EXPECT_TRUE(res.Is(CustomResult::ErrorCode::OtherError));
        EXPECT_FALSE(res.IsOK());
        EXPECT_EQ(error_message, std::string("OtherError"));
    }

    {
        auto res = CustomResultOr<int>::Builder(CustomResult::ErrorCode::OtherError).WithErrorMessage("ddd").Build();
        auto error_message = res.Message();
        EXPECT_TRUE(res.Is(CustomResult::ErrorCode::OtherError));
        EXPECT_FALSE(res.IsOK());
        EXPECT_EQ(error_message, std::string("ddd"));
    }

    {
        CustomResultOr<int> a = 2;
        EXPECT_EQ(2, a.Value());
        EXPECT_EQ(2, a.ValueOr(3));
    }

    {
        CustomResultOr<std::string> a = std::string("2");
        EXPECT_EQ("2", a.Value());
        EXPECT_EQ("2", a.ValueOr("3"));
    }
}

TEST_F(ResultTest, macros_RESULT_OR_RETURN) {
    auto gen_err = [](int x) -> CustomResult {
        if (x == 0) {
            return CustomResult::Builder(CustomResult::ErrorCode::OtherError).Build();
        }

        return CustomResult::Builder(CustomResult::ErrorCode::OK).Build();
    };

    auto f = [&gen_err](int x) -> CustomResult {
        RESULT_OK_OR_RETURN(gen_err(x));

        return CustomResult::Builder(CustomResult::ErrorCode::OK).Build();
    };

    {
        auto res = f(0);
        EXPECT_FALSE(res.IsOK());
    }

    {
        auto res = f(1);
        EXPECT_TRUE(res.IsOK());
    }
}

TEST_F(ResultTest, macros_RESULT_OR_VALUE_RETURN) {
    auto gen_err = [](int x) -> CustomResultOr<int> {
        if (x == 0) {
            return CustomResult::Builder(CustomResult::ErrorCode::OtherError).Build();
        }

        return x;
    };

    auto f = [&gen_err](int x) -> CustomResultOr<int> {
        RESULT_VALUE_OR_RETURN(a, gen_err(x));
        RESULT_VALUE_OR_RETURN(b, gen_err(x));

        EXPECT_EQ(a, x);
        EXPECT_EQ(b, x);

        return a;
    };

    {
        auto res = f(0);
        EXPECT_FALSE(res.IsOK());
    }

    {
        for (int i = 1; i <= 100; i++) {
            auto res = f(i);
            EXPECT_TRUE(res.IsOK());
            EXPECT_EQ(res.Value(), i);
        }
    }
}

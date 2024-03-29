#include "gtest/gtest.h"
#include "snapshot/snapshot.h"

#include "result/macros.h"
#include "result/result.h"
#include "result/result_or.h"
#include "result/types_check/has_error_code_to_str.h"

#include "./custom_result/custom_another_result.h"
#include "./custom_result/custom_result.h"

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
        auto res = CustomResult::OK();
        auto error_message = res.Message();
        EXPECT_TRUE(res.IsOK());
        EXPECT_EQ(error_message, std::string("OK"));
    }

    {
        auto res = CustomResult::Builder(CustomResult::ErrorCode::OtherError).Build();
        auto error_message = res.Message();
        EXPECT_TRUE(res.Is(CustomResult::ErrorCode::OtherError));
        EXPECT_TRUE(res.Is(CustomResult::ErrorCode::CustomError,
                           CustomResult::ErrorCode::OtherError,
                           CustomResult::ErrorCode::NestedError));
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

    {
        auto f = []() -> CustomResult {
            CustomResultOr<int> res = CustomResult::Builder(CustomResult::ErrorCode::OtherError).Build();
            return res;
        };

        EXPECT_TRUE(f().Is(CustomResult::ErrorCode::OtherError));
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
        RESULT_VALUE_OR_RETURN(auto a, gen_err(x));
        RESULT_VALUE_OR_RETURN(auto b, gen_err(x));

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

TEST_F(ResultTest, history_info_test) {
    {
        auto a = []() {
            RESULT_DIRECT_RETURN(CustomResult::Builder(CustomResult::ErrorCode::OtherError).Build());
        };

        auto b = [&a]() {
            RESULT_DIRECT_RETURN(a());
        };

        auto c = [&b]() {
            RESULT_DIRECT_RETURN(b());
        };

        auto res = c();
        EXPECT_EQ(res.HistoryInfoList().size(), 3);
        SNAPSHOT(res.PrettyMessage());
    }

    {
        auto a = []() {
            RESULT_DIRECT_RETURN(CustomResult::Builder(CustomResult::ErrorCode::OtherError).Build());
        };

        auto b = [&a]() -> custom_another_result::CustomAnotherResult {
            RESULT_DIRECT_RETURN(a());
        };

        auto res = b();
        EXPECT_EQ(res.HistoryInfoList().size(), 2);
        SNAPSHOT(res.PrettyMessage());
    }

    {
        auto a = []() {
            RESULT_DIRECT_RETURN(CustomResult::Builder(CustomResult::ErrorCode::OtherError).Build());
        };

        auto b = [&a]() -> custom_another_result::CustomAnotherResult {
            RESULT_DIRECT_RETURN(a());
        };

        auto c = [&b]() {
            RESULT_DIRECT_RETURN(b());
        };

        auto res = c();
        EXPECT_EQ(res.HistoryInfoList().size(), 3);
        SNAPSHOT(res.PrettyMessage());
    }

    {
        auto a = []() {
            RESULT_DIRECT_RETURN(CustomResult::Builder(CustomResult::ErrorCode::OtherError).Build());
        };

        auto b = [&a]() -> custom_another_result::CustomAnotherResult {
            RESULT_DIRECT_RETURN_WITH_NESTED_ERROR(a(), custom_another_result::CustomAnotherResult::ErrorCode::AError);
        };

        auto c = [&b]() {
            RESULT_DIRECT_RETURN(b());
        };

        auto res = c();
        EXPECT_EQ(res.HistoryInfoList().size(), 3);
        SNAPSHOT(res.PrettyMessage());
    }
}

TEST_F(ResultTest, result_to_another_result_test) {
    auto f = []() -> custom_result::CustomResult {
        auto res = custom_another_result::CustomAnotherResult::Builder(
                           custom_another_result::CustomAnotherResult::ErrorCode::OtherError)
                           .Build();
        return res;
    };

    auto res = f();
    EXPECT_FALSE(res.IsOK());
    EXPECT_EQ(res.Code(), custom_result::CustomResult::ErrorCode::NestedError);
    EXPECT_EQ(res.Message(), std::string("OtherError"));
}

TEST_F(ResultTest, result_or_to_another_result_test) {
    auto f = []() -> custom_result::CustomResult {
        custom_another_result::CustomAnotherResultOr<int> a = 1;
        return a;
    };

    auto res = f();
    EXPECT_TRUE(res.IsOK());
    EXPECT_EQ(res.Message(), std::string("OK"));
}

TEST_F(ResultTest, result_direct_return_with_nested_error) {
    {
        auto f = []() -> custom_result::CustomResult {
            auto res = custom_another_result::CustomAnotherResult::Builder(
                               custom_another_result::CustomAnotherResult::ErrorCode::OtherError)
                               .Build();
            RESULT_DIRECT_RETURN_WITH_NESTED_ERROR(res, custom_result::CustomResult::ErrorCode::OtherError);
        };

        auto res = f();
        EXPECT_FALSE(res.IsOK());
        EXPECT_EQ(res.Code(), custom_result::CustomResult::ErrorCode::OtherError);
        EXPECT_EQ(res.HistoryInfoList().size(), 1);
    }

    {
        auto f = []() -> custom_result::CustomResult {
            custom_another_result::CustomAnotherResultOr<int> res =
                    custom_another_result::CustomAnotherResult::Builder(
                            custom_another_result::CustomAnotherResult::ErrorCode::AError)
                            .Build();
            RESULT_DIRECT_RETURN_WITH_NESTED_ERROR(res, custom_result::CustomResult::ErrorCode::BError);
        };

        auto res = f();
        EXPECT_FALSE(res.IsOK());
        EXPECT_EQ(res.Code(), custom_result::CustomResult::ErrorCode::BError);
        EXPECT_EQ(res.HistoryInfoList().size(), 1);
    }

    {
        auto f = []() -> custom_result::CustomResultOr<int> {
            custom_another_result::CustomAnotherResultOr<int> res =
                    custom_another_result::CustomAnotherResult::Builder(
                            custom_another_result::CustomAnotherResult::ErrorCode::AError)
                            .Build();
            RESULT_DIRECT_RETURN_WITH_NESTED_ERROR(res, custom_result::CustomResult::ErrorCode::BError);
        };

        auto res = f();
        EXPECT_FALSE(res.IsOK());
        EXPECT_EQ(res.Code(), custom_result::CustomResult::ErrorCode::BError);
        EXPECT_EQ(res.HistoryInfoList().size(), 1);
    }

    {
        auto f = []() -> custom_result::CustomResultOr<int> {
            auto res = custom_another_result::CustomAnotherResult::Builder(
                               custom_another_result::CustomAnotherResult::ErrorCode::AError)
                               .Build();
            RESULT_DIRECT_RETURN_WITH_NESTED_ERROR(res, custom_result::CustomResult::ErrorCode::BError);
        };

        auto res = f();
        EXPECT_FALSE(res.IsOK());
        EXPECT_EQ(res.Code(), custom_result::CustomResult::ErrorCode::BError);
        EXPECT_EQ(res.HistoryInfoList().size(), 1);
    }

    {
        auto f = []() -> custom_result::CustomResult {
            auto res = custom_another_result::CustomAnotherResult::Builder(
                               custom_another_result::CustomAnotherResult::ErrorCode::AError)
                               .Build();
            RESULT_DIRECT_RETURN_WITH_NESTED_ERROR(res,
                                                   custom_another_result::CustomAnotherResult::ErrorCode::OtherError);
        };

        auto res = f();
        EXPECT_FALSE(res.IsOK());
        EXPECT_EQ(res.Code(), custom_result::CustomResult::ErrorCode::NestedError);
        EXPECT_EQ(res.HistoryInfoList().size(), 1);
    }

    {
        auto f = []() -> custom_result::CustomResultOr<int> {
            auto res = custom_another_result::CustomAnotherResult::Builder(
                               custom_another_result::CustomAnotherResult::ErrorCode::AError)
                               .Build();
            RESULT_DIRECT_RETURN_WITH_NESTED_ERROR(res,
                                                   custom_another_result::CustomAnotherResult::ErrorCode::OtherError);
        };

        auto res = f();
        EXPECT_FALSE(res.IsOK());
        EXPECT_EQ(res.Code(), custom_result::CustomResult::ErrorCode::NestedError);
        EXPECT_EQ(res.HistoryInfoList().size(), 1);
    }

    {
        auto f = []() -> custom_result::CustomResultOr<int> {
            custom_another_result::CustomAnotherResultOr<int> a = 1;
            EXPECT_TRUE(a.HasValue());

            RESULT_DIRECT_RETURN(a);
        };

        auto res = f();
        EXPECT_TRUE(res.IsOK());
        EXPECT_TRUE(res.HasValue());
        EXPECT_EQ(res.Value(), 1);
    }
}

TEST_F(ResultTest, smart_pointer_with_nullptr) {
    {
        auto f = []() -> custom_result::CustomResultOr<std::shared_ptr<int>> {
            return std::make_shared<int>(1);
        };

        auto res = f();
        EXPECT_EQ(*res.Value(), 1);
    }

    {
        auto f = []() -> custom_result::CustomResultOr<std::shared_ptr<int>> {
            return std::shared_ptr<int>(nullptr);
        };

        auto res = f();
        EXPECT_EQ(res.Value(), nullptr);
    }
}

TEST_F(ResultTest, NotOKThenAndAlwaysThen) {
    int flag = 0;

    std::invoke([]() -> CustomResult {
        return CustomResult::OK();
    }).NotOKThen([&flag]([[maybe_unused]] auto&& res) {
        ++flag;
    });

    EXPECT_EQ(flag, 0);

    std::invoke([]() -> CustomResult {
        return CustomResult::Builder(CustomResult::ErrorCode::OtherError).Build();
    }).NotOKThen([&flag]([[maybe_unused]] auto&& res) {
        ++flag;
    });

    EXPECT_EQ(flag, 1);

    std::invoke([]() -> CustomResult {
        return CustomResult::OK();
    }).AlwaysThen([&flag]([[maybe_unused]] auto&& res) {
        ++flag;
    });

    EXPECT_EQ(flag, 2);

    std::invoke([]() -> CustomResult {
        return CustomResult::Builder(CustomResult::ErrorCode::OtherError).Build();
    }).AlwaysThen([&flag]([[maybe_unused]] auto&& res) {
        ++flag;
    });

    EXPECT_EQ(flag, 3);

    {
        int n_flag = 0;
        int a_flag = 0;

        std::invoke([]() -> CustomResult {
            return CustomResult::Builder(CustomResult::ErrorCode::OtherError).Build();
        })
                .NotOKThen([&n_flag](auto&& res) {
                    EXPECT_FALSE(res.IsOK());
                    n_flag = 1;
                })
                .AlwaysThen([&a_flag](auto&& res) {
                    EXPECT_FALSE(res.IsOK());
                    a_flag = 1;
                });

        EXPECT_EQ(n_flag, 1);
        EXPECT_EQ(a_flag, 1);
    }
}

TEST_F(ResultTest, GetHistoryInfoNode) {
    auto res = std::invoke([]() -> CustomResult {
        auto res = custom_another_result::CustomAnotherResult::Builder(
                           custom_another_result::CustomAnotherResult::ErrorCode::OtherError)
                           .Build();
        RESULT_OK_OR_RETURN_WITH_NESTED_ERROR(res, CustomResult::ErrorCode::NestedError);
        return CustomResult::OK();
    });

    auto n = res.GetHistoryInfoNode(1);
    EXPECT_TRUE(n.has_value());
    EXPECT_EQ(n->error_code_num, 1);
    EXPECT_EQ(n->error_code_str, std::string("OtherError"));
}

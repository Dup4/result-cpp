#ifndef RESULT_RESULT_H
#define RESULT_RESULT_H

#include <map>
#include <string>
#include <string_view>

namespace result {

template <typename ErrorCode, std::map<ErrorCode, std::string> ErrorCodeToStr>
class Result {
    using R = Result<ErrorCode, ErrorCodeToStr>;

public:
    Result() = default;
    Result(ErrorCode error_code, std::string_view error_message = "")
            : error_code_(error_code), error_message_(error_message) {}

    Result(const Result&) = default;
    Result& operator=(const Result&) = default;
    Result(Result&&) = default;
    Result& operator=(Result&&) = default;

    bool IsOK() const {
        return error_code_ == ErrorCode::kOK;
    }

    ErrorCode Code() const {
        return error_code_;
    }

    std::string Message() const {
        return error_message_;
    }

public:
    static bool IsError(R& result, ErrorCode error_code) {
        return result.Code() == error_code;
    }

    static R GenerateResult(ErrorCode error_code, std::string_view error_message = "__unset_error_message__") {
        if (error_message == "__unset_error_message__") {
            return R(error_code, ErrorCodeToStr.at(error_code));
        }

        return R(error_code, error_message);
    }

private:
    ErrorCode error_code_{ErrorCode::kOK};
    std::string error_message_{ErrorCodeToStr.at(ErrorCode::kOK)};
};

}  // namespace result

#endif  // RESULT_RESULT_H

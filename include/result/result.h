#ifndef RESULT_RESULT_H
#define RESULT_RESULT_H

#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#include "./internal/visit_external_error_code_to_str.h"
#include "./macros.h"  // IWYU pragma: export
#include "./types_check/has_error_code_to_str.h"

namespace result {

template <typename ErrorCodeType>
class Result {
    using R = Result<ErrorCodeType>;

    static_assert(std::is_enum_v<ErrorCodeType>, "ErrorCode Must be a Enum");
    static_assert(internal::has_error_code_to_str_v<ErrorCodeType>, "ErrorCode Must have a ErrorCodeToStr");

public:
    using ErrorCode = ErrorCodeType;

    static std::string ErrorCodeToStr(ErrorCode error_code) {
        return internal::VisitExternalErrorCodeToStr(error_code);
    }

public:
    struct Options {
        std::optional<std::string> error_message{std::nullopt};
    };

    class Builder {
    public:
        Builder() = delete;
        Builder(ErrorCode error_code) : error_code_(error_code) {}
        ~Builder() = default;

        Builder& WithErrorMessage(const std::string& error_message) {
            options_.error_message = error_message;
            return *this;
        }

        R Build() {
            return R(error_code_, options_.error_message.value_or(ErrorCodeToStr(error_code_)));
        }

    private:
        Options options_;
        ErrorCodeType error_code_;
    };

public:
    Result() = default;
    Result(ErrorCode error_code, std::string_view error_message = "")
            : error_code_(error_code), error_message_(error_message) {}

    Result(const Result&) = default;
    Result& operator=(const Result&) = default;
    Result(Result&&) = default;
    Result& operator=(Result&&) = default;

    ErrorCode Code() const {
        return error_code_;
    }

    std::string Message() const {
        return error_message_;
    }

    bool Is(ErrorCode error_code) const {
        return error_code_ == error_code;
    }

    bool IsOK() const {
        return Is(ErrorCode::OK);
    }

private:
    ErrorCode error_code_{ErrorCode::OK};
    std::string error_message_{ErrorCodeToStr(ErrorCode::OK)};
};

}  // namespace result

#endif  // RESULT_RESULT_H

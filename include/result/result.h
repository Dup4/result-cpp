#ifndef RESULT_RESULT_H
#define RESULT_RESULT_H

#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#include "./types_check/has_error_code_to_str.h"

namespace result {

template <typename ErrorCodeType>
class Result {
    using R = Result<ErrorCodeType>;

    static_assert(std::is_enum_v<ErrorCodeType>, "ErrorCode Must be a Enum");

public:
    using ErrorCode = ErrorCodeType;

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
            if (!options_.error_message.has_value()) {
                if constexpr (internal::has_error_code_to_str_v<ErrorCode>) {
                    options_.error_message = ErrorCodeToStr(error_code_);
                }
            }

            return R(error_code_, options_.error_message.value_or(""));
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
        return Code() == error_code;
    }

    bool IsOK() const {
        return error_code_ == ErrorCode::kOK;
    }

private:
    ErrorCode error_code_{ErrorCode::kOK};
    std::string error_message_{"OK"};
};

}  // namespace result

#endif  // RESULT_RESULT_H

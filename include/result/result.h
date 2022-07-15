#ifndef RESULT_RESULT_H
#define RESULT_RESULT_H

#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "./internal/visit_external_error_code_to_str.h"
#include "./types_check/has_error_code_to_str.h"

#include "./macros.h"  // IWYU pragma: export

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

    struct HistoryInfoNode {
        std::string file_name;
        std::string func_name;
        int32_t line;
    };

    static R OK() {
        return R(ErrorCode::OK, ErrorCodeToStr(ErrorCode::OK));
    }

public:
    Result() = default;
    Result(ErrorCode error_code, std::string_view error_message = "")
            : error_code_(error_code), error_message_(error_message) {}

    virtual ~Result() = default;

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

    int32_t GetErrorCodeInteger() const {
        return static_cast<int32_t>(error_code_);
    }

    std::string GetErrorCodeStr() const {
        return ErrorCodeToStr(error_code_);
    }

    ErrorCode MoveCode() {
        return std::move(error_code_);
    }

    std::string MoveMessage() {
        return std::move(error_message_);
    }

    bool Is(ErrorCode error_code) const {
        return error_code_ == error_code;
    }

    bool IsOK() const {
        return Is(ErrorCode::OK);
    }

    void PushHistory(const std::string& file_name, const std::string& func_name, int32_t line) {
        if (IsOK()) {
            return;
        }

        history_info_list_.emplace_back(HistoryInfoNode{
                .file_name = file_name,
                .func_name = func_name,
                .line = line,
        });

        return;
    }

    std::string GetPrettyHistoryInfo() {
        std::string res = "";

        int ix = 0;

        for (int i = history_info_list_.size() - 1; i >= 0; i--) {
            const auto& info = history_info_list_[i];

            if (ix) {
                res += " -> ";
            }

            ix++;

            res += info.file_name;
            res += ":";
            res += std::to_string(info.line);
        }

        return res;
    }

private:
    ErrorCode error_code_{ErrorCode::OK};
    std::string error_message_{ErrorCodeToStr(ErrorCode::OK)};

private:
    std::vector<HistoryInfoNode> history_info_list_;
};

}  // namespace result

#endif  // RESULT_RESULT_H

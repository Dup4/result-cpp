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
#include "./types_check/is_result.h"

#include "./macros.h"  // IWYU pragma: export

namespace result {

struct HistoryInfoNode {
    std::string file_name;
    int32_t error_code_num;
    std::string error_code_str;
    int32_t line;
};

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

    template <typename T, std::enable_if_t<internal::is_result_v<T> && !std::is_same_v<T, R>, bool> = true>
    Result(T&& t) {
        if (!t.IsOK()) {
            if constexpr (std::is_same_v<decltype(t.Code()), ErrorCode>) {
                error_code_ = t.Code();
            } else {
                error_code_ = ErrorCode::NestedError;
            }

            error_message_ = t.Message();
            history_info_list_ = t.HistoryInfoList();
        }
    }

    template <typename T, std::enable_if_t<internal::is_result_v<T> && !std::is_same_v<T, R>, bool> = true, typename F>
    Result(T&& t, [[maybe_unused]] F&& f) {
        if (!t.IsOK()) {
            if constexpr (std::is_same_v<F, ErrorCode>) {
                error_code_ = f;
            } else {
                error_code_ = ErrorCode::NestedError;
            }

            error_message_ = t.Message();
            history_info_list_ = t.HistoryInfoList();
        }
    }

    ErrorCode Code() const {
        return error_code_;
    }

    int32_t ErrorCodeInteger() const {
        return static_cast<int32_t>(error_code_);
    }

    std::string ErrorCodeStr() const {
        return ErrorCodeToStr(error_code_);
    }

    std::string Message() const {
        return error_message_;
    }

    std::string PrettyMessage() const {
        std::string res = "";
        res += ErrorCodeStr();
        res += ": ";
        res += Message();
        res += "\n";
        res += PrettyHistoryInfo();
        return res;
    }

    template <typename... T, std::enable_if_t<std::conjunction_v<std::is_same<ErrorCode, T>...>, bool> = true>
    bool Is(T&&... t) const {
        return (... || (t == error_code_));
    }

    bool IsOK() const {
        return Is(ErrorCode::OK);
    }

    template <typename F>
    void NotOKThen(F&& f) {
        if (IsOK()) {
            return;
        }

        f(*this);
    }

    template <typename F>
    void AlwaysThen(F&& f) {
        f(*this);
    }

    void PushHistory(const std::string& file_name, int32_t line) {
        if (IsOK()) {
            return;
        }

        history_info_list_.emplace_back(HistoryInfoNode{
                .file_name = file_name,
                .error_code_num = static_cast<int32_t>(error_code_),
                .error_code_str = ErrorCodeStr(),
                .line = line,
        });

        return;
    }

    std::string PrettyHistoryInfo() const {
        std::string res = "";

        int ix = 0;

        for (int i = history_info_list_.size() - 1; i >= 0; i--) {
            const auto& info = history_info_list_[i];

            if (ix) {
                res += "\n";
            }

            ix++;

            res += "(" + info.file_name;
            res += ":";
            res += std::to_string(info.line);
            res += ") ";
            res += info.error_code_str;
        }

        return res;
    }

    const std::vector<HistoryInfoNode>& HistoryInfoList() const {
        return history_info_list_;
    }

    std::optional<HistoryInfoNode> GetHistoryInfoNode(int ix) const {
        if (history_info_list_.size() < size_t(ix)) {
            return std::nullopt;
        }

        return history_info_list_.end()[-ix];
    }

private:
    ErrorCode error_code_{ErrorCode::OK};
    std::string error_message_{ErrorCodeToStr(ErrorCode::OK)};

private:
    std::vector<HistoryInfoNode> history_info_list_;
};

}  // namespace result

#endif  // RESULT_RESULT_H

#pragma once

#include <string>
#include <type_traits>

#include "result/result.h"
#include "result/result_or.h"

namespace custom_result {

enum class ErrorCode {
    OK = 0,
    OtherError,
};

using CustomResult = result::Result<ErrorCode>;

template <typename T>
using CustomResultOr = result::ResultOr<CustomResult, T>;

template <typename T, std::enable_if_t<std::is_same_v<custom_result::ErrorCode, T>, bool> = true>
inline auto ErrorCodeToStr(T error_code) {
    static const std::map<custom_result::ErrorCode, std::string> ErrorCodeToStrMap = {
            {custom_result::ErrorCode::OK, "OK"},
            {custom_result::ErrorCode::OtherError, "OtherError"},
    };

    if (ErrorCodeToStrMap.count(error_code) == 0) {
        return ErrorCodeToStrMap.rbegin()->second;
    }

    return ErrorCodeToStrMap.at(error_code);
}

}  // namespace custom_result

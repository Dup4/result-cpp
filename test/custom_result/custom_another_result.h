#pragma once

#include "result/result.h"
#include "result/result_or.h"

namespace custom_another_result {

enum class ErrorCode {
    OK = 0,
    OtherError,
};

inline auto ErrorCodeToStr(ErrorCode error_code) {
    static const std::map<custom_another_result::ErrorCode, std::string> ErrorCodeToStrMap = {
            {custom_another_result::ErrorCode::OK, "OK"},
            {custom_another_result::ErrorCode::OtherError, "OtherError"},
    };

    if (ErrorCodeToStrMap.count(error_code) == 0) {
        return ErrorCodeToStrMap.rbegin()->second;
    }

    return ErrorCodeToStrMap.at(error_code);
}

using CustomAnotherResult = result::Result<ErrorCode>;

template <typename T>
using CustomAnotherResultOr = result::ResultOr<CustomAnotherResult, T>;

}  // namespace custom_another_result

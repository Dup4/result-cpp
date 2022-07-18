#pragma once

#include <string>
#include <type_traits>

#include "result/result.h"
#include "result/result_or.h"

namespace custom_result {

enum class ErrorCode {
    OK = 0,
    OtherError,
    BError,
    CustomError,
    NestedError,
};

inline auto ErrorCodeToStr(ErrorCode error_code) {
    static const std::map<custom_result::ErrorCode, std::string> ErrorCodeToStrMap = {
            {custom_result::ErrorCode::OK, "OK"},
            {custom_result::ErrorCode::OtherError, "OtherError"},
            {custom_result::ErrorCode::BError, "BError"},
            {custom_result::ErrorCode::CustomError, "CustomError"},
            {custom_result::ErrorCode::NestedError, "NestedError"},
    };

    if (ErrorCodeToStrMap.count(error_code) == 0) {
        return ErrorCodeToStrMap.rbegin()->second;
    }

    return ErrorCodeToStrMap.at(error_code);
}

using CustomResult = result::Result<ErrorCode>;

template <typename T>
using CustomResultOr = result::ResultOr<CustomResult, T>;

}  // namespace custom_result

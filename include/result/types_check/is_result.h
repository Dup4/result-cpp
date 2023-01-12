#ifndef RESULT_TYPES_CHECK_IS_RESULT_H
#define RESULT_TYPES_CHECK_IS_RESULT_H

#include "./has_code.h"
#include "./has_message.h"

namespace result::internal {

template <typename T>
inline constexpr bool is_result_v = has_code_v<T> && has_message_v<T>;

}  // namespace result::internal

#endif  // RESULT_TYPES_CHECK_IS_RESULT_H

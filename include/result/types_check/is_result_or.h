#ifndef RESULT_TYPES_CHECK_IS_RESULT_OR_H
#define RESULT_TYPES_CHECK_IS_RESULT_OR_H

#include "./has_value.h"
#include "./is_result.h"

namespace result::internal {

template <typename T>
inline constexpr bool is_result_or_v = is_result_v<T> && has_value_v<T>;

}  // namespace result::internal

#endif  // RESULT_TYPES_CHECK_IS_RESULT_OR_H

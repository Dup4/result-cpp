#ifndef RESULT_TYPES_CHECK_COMMON_H
#define RESULT_TYPES_CHECK_COMMON_H

namespace result::internal {

template <typename...>
constexpr bool false_v = false;

}

#endif  // RESULT_TYPES_CHECK_COMMON_H

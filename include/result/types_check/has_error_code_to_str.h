#ifndef RESULT_TYPES_CHECK_HAS_ERROR_CODE_TO_STR_H
#define RESULT_TYPES_CHECK_HAS_ERROR_CODE_TO_STR_H

#include <type_traits>

namespace result::internal {

template <typename T>
class has_error_code_to_str {
private:
    template <typename U>
    static constexpr auto check(int) -> decltype(ErrorCodeToStr(std::declval<U>()), std::true_type());

    template <typename>
    static constexpr std::false_type check(...);

    typedef decltype(check<T>(0)) type;

public:
    static constexpr bool value = type::value;
};

template <typename T>
inline constexpr bool has_error_code_to_str_v = has_error_code_to_str<T>::value;

}  // namespace result::internal

#endif  // RESULT_TYPES_CHECK_HAS_ERROR_CODE_TO_STR_H

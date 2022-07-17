#ifndef RESULT_TYPES_CHECK_HAS_CODE_H
#define RESULT_TYPES_CHECK_HAS_CODE_H

#include <type_traits>

namespace result::internal {

template <typename T>
class has_code {
private:
    template <typename U>
    static constexpr auto check(int) -> decltype(std::declval<U>().Code(), std::true_type());

    template <typename>
    static constexpr std::false_type check(...);

    typedef decltype(check<T>(0)) type;

public:
    static constexpr bool value = type::value;
};

template <typename T>
inline constexpr bool has_code_v = has_code<T>::value;

}  // namespace result::internal

#endif  // RESULT_TYPES_CHECK_HAS_CODE_H

#ifndef RESULT_TYPES_CHECK_HAS_VALUE_H
#define RESULT_TYPES_CHECK_HAS_VALUE_H

#include <type_traits>

namespace result::internal {

template <typename T>
class has_value {
private:
    template <typename U>
    static constexpr auto check(int) -> decltype(std::declval<U>().Value(), std::true_type());

    template <typename>
    static constexpr std::false_type check(...);

    typedef decltype(check<T>(0)) type;

public:
    static constexpr bool value = type::value;
};

template <typename T>
inline constexpr bool has_value_v = has_value<T>::value;

}  // namespace result::internal

#endif  // RESULT_TYPES_CHECK_HAS_VALUE_H

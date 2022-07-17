#ifndef RESULT_MACROS_IMPL_H
#define RESULT_MACROS_IMPL_H

#define __RESULT_DIRECT_RETURN_IMPL(func, res_name)         \
    auto res_name = func;                                   \
                                                            \
    if (!res_name.IsOK()) {                                 \
        res_name.PushHistory(__FILE__, __func__, __LINE__); \
    }                                                       \
                                                            \
    return res_name;

#define __RESULT_DIRECT_RETURN_WITH_NESTED_ERROR(func, nested_error_code, res_name) \
    auto res_name = func;                                                           \
                                                                                    \
    if (!res_name.IsOK()) {                                                         \
        res_name.PushHistory(__FILE__, __func__, __LINE__);                         \
    }                                                                               \
                                                                                    \
    return res_name;

#define __RESULT_OK_OR_RETURN_IMPL(func, res_name)          \
    auto res_name = func;                                   \
                                                            \
    if (!res_name.IsOK()) {                                 \
        res_name.PushHistory(__FILE__, __func__, __LINE__); \
        return res_name;                                    \
    }

#define __RESULT_VALUE_OR_RETURN_IMPL(var_name, func, res_name) \
    auto res_name = func;                                       \
                                                                \
    if (!res_name.IsOK()) {                                     \
        res_name.PushHistory(__FILE__, __func__, __LINE__);     \
        return res_name;                                        \
    }                                                           \
                                                                \
    var_name = std::move(res_name.Value());

#define __RESULT_GENERATE_RES_NAME(line, counter) __RESULT_GENERATE_RES_NAME_(line, counter)
#define __RESULT_GENERATE_RES_NAME_(line, counter) __res_##line##_##counter##_

#endif  // RESULT_MACROS_IMPL_H

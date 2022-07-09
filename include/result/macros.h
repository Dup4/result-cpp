#ifndef RESULT_MACROS_H
#define RESULT_MACROS_H

#define RESULT_OR_RETURN(func) __RESULT_OR_RETURN_IMPL(func, __RESULT_GENERATE_RES_NAME(__LINE__, __COUNTER__))

#define __RESULT_OR_RETURN_IMPL(func, res_name) \
    auto res_name = func;                       \
    if (!res_name.IsOK()) {                     \
        return res_name;                        \
    }

#define RESULT_VALUE_OR_RETURN(var_name, func) \
    __RESULT_VALUE_OR_RETURN_IMPL(var_name, func, __RESULT_GENERATE_RES_NAME(__LINE__, __COUNTER__))

#define __RESULT_VALUE_OR_RETURN_IMPL(var_name, func, res_name) \
    auto res_name = func;                                       \
    if (!res_name.IsOK()) {                                     \
        return res_name;                                        \
    }                                                           \
                                                                \
    auto var_name = std::move(res_name.Value());

#define __RESULT_GENERATE_RES_NAME(line, counter) __RESULT_GENERATE_RES_NAME_(line, counter)
#define __RESULT_GENERATE_RES_NAME_(line, counter) __res_##line##_##counter##_

#endif  // RESULT_MACROS_H

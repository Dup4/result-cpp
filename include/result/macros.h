#ifndef RESULT_MACROS_H
#define RESULT_MACROS_H

#define RESULT_MUST(var_name, func) RESULT_MUST_IMPL(var_name, func, __res_##__LINE__##_##__COUNTER__##_)

#define RESULT_MUST_IMPL(var_name, func, res_name) \
    auto res_name = func;                          \
    if (!res_name.IsOK()) {                        \
        return res_name;                           \
    }                                              \
                                                   \
    auto var_name = std::move(res_name.Value());

#endif  // RESULT_MACROS_H

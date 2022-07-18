#ifndef RESULT_MACROS_H
#define RESULT_MACROS_H

#include "./macros_impl.h"

#define RESULT_DIRECT_RETURN(func) __RESULT_DIRECT_RETURN_IMPL(func, __RESULT_GENERATE_RES_NAME(__LINE__, __COUNTER__))

#define RESULT_DIRECT_RETURN_WITH_NESTED_ERROR(func, nested_error_code) \
    __RESULT_DIRECT_RETURN_WITH_NESTED_ERROR_IMPL(                      \
            func, nested_error_code, __RESULT_GENERATE_RES_NAME(__LINE__, __COUNTER__))

#define RESULT_OK_OR_RETURN(func) __RESULT_OK_OR_RETURN_IMPL(func, __RESULT_GENERATE_RES_NAME(__LINE__, __COUNTER__))

#define RESULT_OK_OR_RETURN_WITH_NESTED_ERROR(func, nested_error_code) \
    __RESULT_OK_OR_RETURN_WITH_NESTED_ERROR_IMPL(                      \
            func, nested_error_code, __RESULT_GENERATE_RES_NAME(__LINE__, __COUNTER__))

#define RESULT_VALUE_OR_RETURN(var_name, func) \
    __RESULT_VALUE_OR_RETURN_IMPL(var_name, func, __RESULT_GENERATE_RES_NAME(__LINE__, __COUNTER__))

#define RESULT_VALUE_OR_RETURN_WITH_NESTED_ERROR(var_name, func, nested_error_code) \
    __RESULT_VALUE_OR_RETURN_WITH_NESTED_ERROR_IMPL(                                \
            var_name, func, nested_error_code, __RESULT_GENERATE_RES_NAME(__LINE__, __COUNTER__))

#endif  // RESULT_MACROS_H

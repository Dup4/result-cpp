#ifndef RESULT_MACROS_H
#define RESULT_MACROS_H

#include "./macros_impl.h"

#define RESULT_OR_RETURN(func) __RESULT_OR_RETURN_IMPL(func, __RESULT_GENERATE_RES_NAME(__LINE__, __COUNTER__))

#define RESULT_VALUE_OR_RETURN(var_name, func) \
    __RESULT_VALUE_OR_RETURN_IMPL(var_name, func, __RESULT_GENERATE_RES_NAME(__LINE__, __COUNTER__))

#endif  // RESULT_MACROS_H

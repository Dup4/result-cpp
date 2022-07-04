#ifndef RESULT_INTERNAL_VISIT_EXTERNAL_ERROR_CODE_TO_STR
#define RESULT_INTERNAL_VISIT_EXTERNAL_ERROR_CODE_TO_STR

#include <string>

namespace result::internal {

template <typename T>
inline std::string VisitExternalErrorCodeToStr(const T& t) {
    return ErrorCodeToStr(t);
}

}  // namespace result::internal

#endif  // RESULT_INTERNAL_VISIT_EXTERNAL_ERROR_CODE_TO_STR

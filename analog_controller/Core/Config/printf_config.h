#ifndef PRINTF_CONFIG_H
#define PRINTF_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#define PRINTF_USE_DOUBLE_INTERNALLY              0
#define PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS     0
#define SUPPORT_MSVC_STYLE_INTEGER_SPECIFIERS     0
#define PRINTF_ALIAS_STANDARD_FUNCTION_NAMES_SOFT 1
#define PRINTF_IMPLICIT_CR_ON_LF                  1

#ifdef __cplusplus
}
#endif

#endif // PRINTF_CONFIG_H
/**
 * @brief Error Code Definitions
 */


#ifndef ERROR_H
#define ERROR_H

#include "stdint.h"

typedef enum {
    OK = (int8_t) 0,
    INVALID_ARG = (int8_t) (-1),
    NULL_POINTER = (int8_t) (-2)
} error_t;


#endif
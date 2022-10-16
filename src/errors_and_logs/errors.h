#pragma once

#include "logs.h"
#include "../stack/stack.h"

//------------------------------------------------------------------

enum general_errors {
    OPEN_FILE_ERR  = -2,
    CLOSE_FILE_ERR = -3,
    NULL_PTR_ERR   = -4,
    NO_MEMORY_ERR  = -5,
    RECALLOC_ERR   = -6,
    RESIZE_MOD_ERR = -7,
    SET_DATA_ERR   = -8,
    MEMSET_ERR     = -9,
};

//------------------------------------------------------------------

#define print_error(...) \
        _print_error(LOCATION, __VA_ARGS__)

//------------------------------------------------------------------

int _print_error ( const char *file, const unsigned line, const char *func, const char *format, ...);

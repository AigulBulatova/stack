#pragma once

#include "../stack/stack.h"

#include <stdarg.h>


#define print_log_messsage(...) \
        _print_log_messsage(LOCATION, __VA_ARGS__)

//------------------------------------------------------------------

int open_logfile (const char *filename, const char *mode);

int close_logfile ();

int _print_log_messsage (const char *file, const unsigned line, const char *func, char *format, ...);

int print_to_log (const char *format, ...);

// TODO 
// int print_err_msg(int err_code, const char* err_str = NULL, ...)
// если не компилится, то = NULL убрать -> 



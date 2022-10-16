#pragma once

#include "../stack/stack.h"

//------------------------------------------------------------------

#define print_log_messsage(...) \
        _print_log_messsage(LOCATION, __VA_ARGS__)

//------------------------------------------------------------------

int open_logfile (const char *filename, const char *mode);

int close_logfile ();

int _print_log_messsage (const char *file, const unsigned line, const char *func, char *format, ...);

int print_to_log (const char *format, ...);




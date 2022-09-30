#pragma once

#include "../stack/stack.h"


#define print_log_messsage(...) \
        _print_log_messsage(LOCATION, __VA_ARGS__)

//------------------------------------------------------------------

int open_logfile ();

int close_logfile ();

int _print_log_messsage (const char *file, const unsigned line, const char *func, char *format, ...);





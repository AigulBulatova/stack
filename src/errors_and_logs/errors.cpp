#include <assert.h>
#include <stdarg.h>

#include "errors.h"

//------------------------------------------------------------------

int _print_error ( const char *file, const unsigned line, const char *func, const char *format, ...)
{
    assert (file);
    assert (func);
    assert (format);

    fprintf (stderr, "Error message from file %s, function %s (line %u): ", file, func, line);

    va_list args = {};

    va_start (args, format);
    vfprintf (stderr, format, args);

    va_end (args);

    fprintf (stderr, "\n");

    return 0;
    
}

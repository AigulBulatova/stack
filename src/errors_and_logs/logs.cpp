#include <stdio.h>
#include <assert.h>

#include "errors.h"

//------------------------------------------------------------------

static FILE *log_file = NULL;

//------------------------------------------------------------------

int open_logfile (const char *filename, const char *mode)
{
    if (filename == NULL) {
        fprintf (stderr, "Null pointer to log filename.\n");
        return OPEN_FILE_ERR;
    }

    if (mode == NULL) {
        fprintf (stderr, "Null pointer to log file opening mode.\n");
        return OPEN_FILE_ERR;
    }

    log_file = fopen (filename, mode);

    if (log_file == NULL) {
        fprintf (stderr, "Can not open log file.\n");   
        return OPEN_FILE_ERR;
    }

    return 0;
}

//------------------------------------------------------------------

int close_logfile ()
{
    if (log_file == NULL) {
        fprintf (stderr, "Null pointer to logfile.\n");
        return CLOSE_FILE_ERR;
    }   

    int close = fclose (log_file);
    if (close == EOF) {
        fprintf (stderr, "Can not close log file.\n");
        return close;
    }

    return 0;
}

//------------------------------------------------------------------

int _print_log_messsage (const char *file, const unsigned line, const char *func, char *format, ...)
{
    assert (format);
    assert (file);
    assert (func);

    fprintf (log_file, "Log message from file %s, function %s (line number %ul):\n",
                         file, func, line);

    va_list args = {};

    va_start (args, format);
    vfprintf (log_file, format, args);

    va_end (args);

    fprintf (log_file, "\n");

    return 0;
}

//------------------------------------------------------------------

int print_to_log (const char *format, ...) 
{
    assert (format);

    va_list args = {};

    va_start (args, format);
    vfprintf (log_file, format, args);

    va_end (args);

    fprintf (log_file, "\n");

    return 0;
}
#include <stdio.h>

//------------------------------------------------------------------

static FILE *log_file = NULL;

//------------------------------------------------------------------

int open_logfile ()
{
    log_file = fopen ("errors_and_logs/log_file.txt", "w")
    if (log_file == NULL) {
        //print
        return ERROR; /////////
    }
}

//------------------------------------------------------------------

void _stack_dump (Stack *stack, const char *file, const unsigned int line, const char *func)
{
    fprintf (log_file, "Stack_dump was called from %s at file %s, line number %d:\n", func, file, line);
    fprintf (log_file, "Stack [%p]", stack);

    Info *info = stack->info;
    assert(info);

    fprintf (log_file, " \"%s\" at %s at file %s(%d)\n", 
                        info->name, info->function, info->file, info->line);

    int verify = stack_verify (stack);

    if (verify != 0) {
        print_error (verify);
    }

    fprintf (log_file, "{\n\t size = %lu\n\t capacity = %lu\n\t data = [%p]\n {\n", 
                        stack->size, stack->capacity, stack->data);

    for (int i = 0; i < stack->capacity; i++) {
        fprintf (log_file, "\t\t[%d] = %lf\n", i, stack->data[i]);           //доработать
    }
    fprintf (log_file, "}\n");

}
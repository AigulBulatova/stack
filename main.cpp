#include "src/stack/stack.h"
#include "src/errors_and_logs/errors.h"
#include "src/errors_and_logs/logs.h"
#include "config.h"

int main ()
{
    int error = 0;

    #ifdef DEBUG

        error = open_logfile ("src/errors_and_logs/log_file.txt", "w");
        if (error < 0) return error;
        
    #endif
    
    Stack stk1 = {};
    stack_ctor (&stk1);
    //printf ("%s  %s  %s  %u\n", stk1.info->name, stk1.info->func, stk1.info->file, stk1.info->line);
    
    error = stack_push (&stk1, 5);
    if (error < 0) {
        printf ("ERROR\n");
    }

    _stack_dump(&stk1, __FILE__, __LINE__, __FUNCTION__);

    stack_dtor (&stk1);

    return 0;
}
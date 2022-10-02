#include "stack/stack.h"
#include "errors_and_logs/errors.h"
#include "stack/stack.h"
#include "errors_and_logs/logs.h"
#include "config.h"

int main ()
{
    int error = 0;

    #ifdef DEBUG
        error = open_logfile ("errors_and_logs/log_file.txt", "w");
        if (error < 0) return error;
    #endif

    
    Stack stk1 = {};
    stack_ctor (&stk1);
}
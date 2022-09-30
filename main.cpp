#include "stack/stack.h"
#include "errors_and_logs/errors_and_logs.h"

int main ()
{
    #ifdef DEBUG
        err = open_logfile ("errors_and_logs/log_file.txt", "w");
        if (err < 0) return err;
    #endif

    
    Stack stk1 = {};
    stack_ctor (stk1);
}
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
    error = stack_push (&stk1, 5);
    //stack_push (&stk1, 6);
    //stack_push (&stk1, 7);

    //_stack_dump(&stk1, __FILE__, __LINE__, __FUNCTION__);

    return 0;
}
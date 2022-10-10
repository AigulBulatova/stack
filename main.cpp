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
    if (error < 0) {
        printf ("ERROR\n");
    }

    error = stack_push (&stk1, 6);
    if (error < 0) {
        printf ("ERROR\n");
    }

    error = stack_push (&stk1, 10);
    if (error < 0) {
        printf ("ERROR\n");
    }

    elem_t a = stack_pop(&stk1);

    printf ("%lf\n", a);

    _stack_dump(&stk1, __FILE__, __LINE__, __FUNCTION__);

    stack_dtor (&stk1);

    #ifdef DEBUG

        error = close_logfile ();
        if (error < 0) return error;
        
    #endif

    return 0;
}
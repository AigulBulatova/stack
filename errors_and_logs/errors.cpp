#include "errors.h"

//------------------------------------------------------------------

#ifdef DEBUG

int _stack_dump (Stack *stack, const char *file, const unsigned line, const char *func)
{
    fprintf (log_file, "Stack_dump was called from %s at file %s, line number %d:\n", func, file, line);
    fprintf (log_file, "Stack [%p]", stack);

    Var_info *info = stack->info;
    assert(info);

    fprintf (log_file, " \"%s\" at %s at file %s(%d):\n", 
                        info->name, info->function, info->file, info->line);


    print_error (stack->error_code);
    
    fprintf (log_file, "See more information about stack %s", info->name);
    fprintf (log_file, "{\n\t size = %lu\n\t capacity = %lu\n\t data = [%p]\n {\n", 
                        stack->size, stack->capacity, stack->data);

    for (int i = 0; i < stack->capacity; i++) {

        if (stack->data[i] == POISON_VALUE) {
            fprintf (log_file, "\t\t*[%d] = %lf\t(POISON)\n", i, stack->data[i])
        }

        else {
            fprintf (log_file, "\t\t[%d] = %lf\n", i, stack->data[i]);    
        }      
    }

    #ifdef CANARIES

        fprintf (log_file, "Values of canaries: \n");
        
        fprintf (log_file, "\tStack canaries: \n\t\tstack_canary1 = %d \n\t\tstack_canary2 = %d\n", stack->canary1, stack->canary2);

        int64_t *data_canary1 = (int64_t *) ((char *)stack->data - sizeof(int64_t));
        int64_t *data_canary2 = (int64_t *) ((char *)stack->data + stack->capacity);

        fprintf (log_file, "\tData canaries: \n\t\tdata_canary1 = %d \n\t\t data_canary2 = %d\n", *data_canary1, *data_canary2);
    
    #endif

    #ifdef HASH


    #endif

    fprintf (log_file, "}\n");

    return 0;
}

#endif

//------------------------------------------------------------------

#ifdef DEBUG

int _stack_verify (Stack *stack, const char *file, const unsigned line, const char *function)
{
    int errors = 0;

    if (stack == NULL) {
        errors += STK_NULL_PTR;
    }

    if (stack->size < 0) {
        errors += STK_INV_SIZE;
    }

    if (stack->capacity < 0) {
        errors += STK_INV_CAPACITY;
    }

    if (stack->size > stack->capacity) {
        errors += STK_CAP_LESS_SIZE;
    }

    if (stack->size >= MAXCAPACITY || stack->capacity > MAXCAPACITY) {
        errors += STK_
    }

    #ifdef CANARIES


    #endif

    stack->error_code += errors;

    if (errors != 0) {
        stack_dump (stack, file, line, function);
        
    }
    
    return 0;
}

#endif

//------------------------------------------------------------------

#ifdef DEBUG

int stack_pop_verify (Stack *stack, const char *file, const unsigned line, const char *function)
{
    int errors = 0;

    if (stack->capacity == 0 || stack->size == 0) {
        errors += STK_POP_ERR;
    }

    /////////
    stack->error_code += errors;

    _stack_verify (stack, file, line, function);

    return 0;
}

#endif

//------------------------------------------------------------------

int print_stack_error (int errors)
{
    if (errors & STK_NULL_PTR) {
        fprintf (log_file, "Null pointer to stack.\n");
    }

    if (errors & STK_INV_CAPACITY) {
        fprintf (log_file, "Invalid capacity of the stack.\n");
    }

    if (errors & STK_INV_SIZE) {
        fprintf (log_file, "Invalid size of the stack.\n");
    }

    if (errors & STK_CAP_LESS_SIZE) {
        fprintf (log_file, "Stack capacity less than stack size.\n");
    }

    if (errors & STK_OVERFLOW) {
        fprintf (log_file, "Stack overflow.\n");

    }

}

//------------------------------------------------------------------

int print_error (int errors)
{

}

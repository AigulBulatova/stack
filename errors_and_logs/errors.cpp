#include "errors.h"
#include "logs.h"
#include "../stack_hash/stack_hash.h"

#include <assert.h>

//------------------------------------------------------------------

extern FILE *log_file;

#ifdef DEBUG

int _stack_dump (Stack *stack, const char *file, const unsigned line, const char *func)
{
    fprintf (log_file, "Stack_dump was called from %s at file %s, line number %ud:\n", func, file, line);
    fprintf (log_file, "Stack [%p]", stack);

    Var_info *info = stack->info;
    assert(info);

    fprintf (log_file, " \"%s\" at %s at file %s(%ud):\n", 
                        info->name, info->function, info->file, info->line);


    print_error (stack->error_code);
    
    fprintf (log_file, "See more information about stack %s", info->name);
    fprintf (log_file, "{\n\t size = %lu\n\t capacity = %lu\n\t data = [%p]\n {\n", 
                        stack->size, stack->capacity, stack->data);

    for (int i = 0; i < stack->capacity; i++) {

        if (stack->data[i] == POISON_VALUE) {
            fprintf (log_file, "\t\t*[%d] = ", i);
            fprintf (log_file, TYPE_SPEC, stack->data[i]);
            fprintf (log_file, "\t(POISON)\n");
        }

        else {
            fprintf (log_file, "\t\t[%d] = ", i); 
            fprintf (log_file, TYPE_SPEC, stack->data[i]);
            fprintf (log_file, "\t\n");   
        }      
    }

    #ifdef CANARIES

        fprintf (log_file, "Values of canaries: \n");
        
        fprintf (log_file, "\tStack canaries: \n\t\tstack_canary1 = %ld \n\t\tstack_canary2 = %ld\n", stack->canary1, stack->canary2);

        int64_t *data_canary1 = (int64_t *) ((char *)stack->data - sizeof(int64_t));
        int64_t *data_canary2 = (int64_t *) ((char *)stack->data + stack->capacity);

        fprintf (log_file, "\tData canaries: \n\t\tdata_canary1 = %ld \n\t\t data_canary2 = %ld\n", *data_canary1, *data_canary2);
    
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
        errors += STK_STRUCT_NULL_PTR;
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
        errors += STK_OVERFLOW;
    }


    #ifdef CANARIES


    #endif

    stack->error_code += errors;

    if (errors != 0) {

        _stack_dump (stack, file, line, function);
        return -1;

    }
    
    return 0;
}

#endif

//------------------------------------------------------------------

#ifdef DEBUG

int _stack_pop_verify (Stack *stack, const char *file, const unsigned line, const char *function)
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

#ifdef HASH

int stack_struct_hash_chack (Stack *stack)
{
    unsigned int struct_len = sizeof (Stack) - 2 * sizeof (int64_t);

    int64_t struct_hash = get_hash (stack, struct_len);

    if (struct_len != stack->stack_hash) {
        return -1; ////hash_check_err???
    }

    return 0;
}

#endif

//------------------------------------------------------------------

#ifdef HASH

int stack_data_hash_check (Stack *stack)
{
    #ifdef CANARIES

        char *data = (char *)stack->data - sizeof (int64_t);
        unsigned int data_len = stack->capacity * sizeof (elem_t) + 2 * sizeof (int64_t);
    
    #else 

        char *data = (char *) stack->data;
        unsigned int data_len = stack->capacity * sizeof (elem_t);

    #endif

    int64_t data_hash = get_hash (data, data_len);

    if (data_hash != stack->data_hash) {
        return -1;
    } 

    return 0;
}

#endif

//------------------------------------------------------------------

int print_stack_error (int errors)
{
    if (errors & STK_STRUCT_NULL_PTR) {
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

    if (errors & STK_SET_DATA_ERR) {
        fprintf (log_file, "");
    }

    if (errors & STK_DATA_NULL_PTR) {
        fprintf (log_file, "");
    }

    if (errors & STK_RESIZE_MOD_ERR) {
        fprintf (log_file, "");
    }

    if (errors & STK_STRUCT_CANARY_ERR) {
        fprintf (log_file, "");
    }

    if (errors & STK_DATA_CANARY_ERR) {
        fprintf (log_file, "");
    }

    if (errors & STK_STRUCK_HASH_ERR) {
        fprintf (log_file, "");
    }
    
    if (errors & STK_DATA_HASH_ERR) {
        fprintf (log_file, "");
    }

    return 0;
}

//------------------------------------------------------------------

int _print_error (int error, FILE *err_file, const char *file, const unsigned line, const char *func)
{
    fprintf (err_file, "Error message from file %s, function %s (line %ud): ", file, func, line);

    switch (error) {
        case OPEN_FILE_ERR: {

            fprintf (err_file, "Can not open file.\n");
            break;

        }

        case CLOSE_FILE_ERR: {
            
            fprintf (err_file, "Can not close file.\n");
            break;

        }

        case NULL_PTR_ERR: {

            fprintf (err_file, ""); //////////////////
            break;

        }

        case NO_MEMORY_ERR: {

            fprintf (err_file, "Can not allocate memory.\n");
            break;

        }


        default: {
            fprintf (err_file, "Unexpected error code.\n");
        }
    }
}

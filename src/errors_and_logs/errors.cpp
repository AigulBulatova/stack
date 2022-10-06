#include "errors.h"
#include "logs.h"
#include "../stack_hash/stack_hash.h"
#include "../../config.h"

#include <assert.h>

//------------------------------------------------------------------

#ifdef DEBUG

int _stack_dump (Stack *stack, const char *file, const unsigned line, const char *func)
{
    print_to_log ("Stack_dump was called from %s at file %s, line number %u:\n", func, file, line);
    print_to_log ("Stack [%p]", stack);

    Var_info *info = stack->info;
    assert(info);
    printf ("hi\n");
    print_to_log (" \"%s\" at %s at file %s(%u):\n", 
                        info->name, info->func, info->file, info->line);


    // print_error (stack->error_code);
    
    print_to_log ("See more information about stack %s", info->name);
    print_to_log ("{\n\t size = %lu\n\t capacity = %lu\n\t data = [%p]\n {\n", 
                        stack->size, stack->capacity, stack->data);

    for (int i = 0; i < stack->capacity; i++) {

        if (stack->data[i] == POISON_VALUE) {
            print_to_log ("\t\t*[%d] = ", i);
            print_to_log ("%lf", stack->data[i]);
            print_to_log ("\t(POISON)\n");
        }

        else {
            print_to_log ("\t\t[%d] = ", i); 
            print_to_log (TYPE_SPEC, stack->data[i]);
            print_to_log ("\n");   
        }      
    }

    print_to_log ("}\n");

    #ifdef CANARIES

        print_to_log ("Values of canaries: \n");
        
        print_to_log ("\tStack canaries: \n\t\tstack_canary1 = %ld \n\t\tstack_canary2 = %ld\n", stack->canary1, stack->canary2);

    if (stack->data != (elem_t *) NOT_ALLOC_YET_PTR) {
        int64_t *data_canary1 = (int64_t *) ((char *)stack->data - sizeof(int64_t));
        int64_t *data_canary2 = (int64_t *) ((char *)stack->data + stack->capacity);

        print_to_log ("\tData canaries: \n\t\tdata_canary1 = %ld \n\t\t data_canary2 = %ld\n", *data_canary1, *data_canary2);
    }

    #endif

    #ifdef HASH


    #endif

    print_to_log ("}\n");

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

        errors += stack_canary_verify (stack);

    #endif

    #ifdef HASH 

        if (stack_struct_hash_check (stack) == -1) {
            errors += STK_STRUCT_HASH_ERR;
        }

        if (stack->data != NULL &&
            stack->data != (elem_t *) NOT_ALLOC_YET_PTR &&
            stack_data_hash_check (stack) == -1) {

            errors += STK_DATA_HASH_ERR;
        }

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

    if (stack->capacity == 0 || stack->size == 0) { // TODO: emplace in pop 
        errors += STK_POP_ERR;
    }

    stack->error_code += errors;

    _stack_verify (stack, file, line, function);

    return 0;
}

#endif

//------------------------------------------------------------------

#ifdef HASH

int stack_struct_hash_check (Stack *stack)
{
    unsigned long struct_len = sizeof (Stack) - 2 * sizeof (int64_t);

    int64_t struct_hash = get_hash (stack, struct_len);

    if (struct_hash != stack->stack_hash) {
        //printf ("%ld    %ld\n", struct_hash, stack->stack_hash);
        return -1;
    }

    return 0;
}

#endif

//------------------------------------------------------------------

#ifdef HASH

int stack_data_hash_check (Stack *stack)
{
    if (stack->data == NULL || stack->data == (elem_t *) NOT_ALLOC_YET_PTR) {
        return 0;
    }

    #ifdef CANARIES

        char *data = (char *)stack->data - sizeof (int64_t);
        unsigned long data_len = stack->capacity * sizeof (elem_t) + 2 * sizeof (int64_t);
    
    #else 

        char *data = (char *) stack->data;
        unsigned long data_len = stack->capacity * sizeof (elem_t);

    #endif

    int64_t data_hash = get_hash (data, data_len);

    if (data_hash != stack->data_hash) {
        return -1;
    } 

    return 0;
}

#endif

//------------------------------------------------------------------

#ifdef CANARIES

int stack_canary_verify (Stack *stack)
{
    int error_code = 0;

    if (stack->canary1 != CANARY_VALUE) {
        error_code += STK_STRUCT_CANARY1_ERR;
    }

    if (stack->canary2 != CANARY_VALUE) {
        error_code += STK_STRUCT_CANARY2_ERR;
    }

    if (stack->data != NULL && 
        stack->data != (elem_t *) NOT_ALLOC_YET_PTR) {

            int64_t *data_canary = (int64_t *) stack->data - 1;

            if (*data_canary != CANARY_VALUE) {
                error_code += STK_DATA_CANARY1_ERR;
            }

            data_canary = (int64_t *) (stack->data + stack->capacity);

            if (*data_canary != CANARY_VALUE) {
                error_code += STK_DATA_CANARY2_ERR;
            }
        }
    
    return error_code;
}

#endif

//------------------------------------------------------------------

int print_stack_error (int errors)
{
    if (errors & STK_STRUCT_NULL_PTR) {
        print_to_log ("Null pointer to stack.\n");
    }

    if (errors & STK_INV_CAPACITY) {
        print_to_log ("Invalid capacity of the stack.\n");
    }

    if (errors & STK_INV_SIZE) {
        print_to_log ("Invalid size of the stack.\n");
    }

    if (errors & STK_CAP_LESS_SIZE) {
        print_to_log ("Stack capacity less than stack size.\n");
    }

    if (errors & STK_OVERFLOW) {
        print_to_log ("Stack overflow.\n");
    }

    if (errors & STK_DATA_NULL_PTR) {
        print_to_log ("Null pointer to stack data.\n");
    }

    if (errors & STK_STRUCT_CANARY1_ERR) {
        print_to_log ("Canary protection was broken with left canary on stack struct.\n");
    }

    if (errors & STK_STRUCT_CANARY2_ERR) {
        print_to_log ("Canary protection was broken with right canary on stack struct.\n");
    }

    if (errors & STK_DATA_CANARY1_ERR) {
        print_to_log ("Canary protection was broken with left canary on stack data.\n");
    }

    if (errors & STK_DATA_CANARY1_ERR) {
        print_to_log ("Canary protection was broken with right canary on stack data.\n");
    }

    if (errors & STK_STRUCT_HASH_ERR) {
        print_to_log ("Stack struct hash protection was broken.\n");
    }
    
    if (errors & STK_DATA_HASH_ERR) {
        print_to_log ("Stack data protection was broken.\n");
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

            fprintf (err_file, "Null pointer.\n"); //////////////////
            break;
        }

        case NO_MEMORY_ERR: {

            fprintf (err_file, "Can not allocate memory.\n");
            break;
        }

        case RESIZE_MOD_ERR: {

            fprintf (err_file, "Unexpected resize mode.\n");
            break;
        }

        case SET_DATA_ERR: {

            fprintf (err_file, "Invalid size to set stack data.\n");
            break;
        }
        
        default: {
            fprintf (err_file, "Unexpected error code.\n");
        }
    }

    return 0;
}

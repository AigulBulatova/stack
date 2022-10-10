#include <assert.h>
#include <math.h>

#include "errors.h"
#include "logs.h"
#include "../stack_hash/stack_hash.h"
#include "../../config.h"
#include "../general/general.h"

//------------------------------------------------------------------

#ifdef DEBUG

int _stack_dump (Stack *stack, const char *file, const unsigned line, const char *func)
{
    print_to_log ("-----------------------------------------------------------------------------\n");

    print_to_log ("Stack_dump was called from %s at file %s, line number %u:\n", func, file, line);
    print_to_log ("Stack [%p]", stack);

    Var_info *info = stack->info;
    assert(info);

    print_to_log (" \"%s\" at %s at file %s(%u):\n", 
                        info->name, info->func, info->file, info->line);


    print_stack_error (stack->error_code);
    
    print_to_log ("See more information about stack %s", info->name);
    print_to_log ("{\n\t size = %lu\n\t capacity = %lu\n\t data = [%p]\n\n\t{\n", 
                        stack->size, stack->capacity, stack->data);

    for (int i = 0; i < stack->capacity; i++) {

        if (fabs (stack->data[i] - POISON_VALUE) <= 1e-6) {
            print_to_log ("\t\t*[%d] = " TYPE_SPEC " \t(POISON) ", i, stack->data[i]);
            print_to_log ("\n");
        }

        else {
            print_to_log ("\t\t[%d] = " TYPE_SPEC " ", i, stack->data[i]); 
            print_to_log ("\n");   
        }      
    }

    print_to_log ("\t}\n}\n");

    #ifdef CANARIES

        print_to_log ("Values of canaries: \n");
        
        print_to_log ("\tStack canaries: \n\t\tstack_canary1 = %ld \n\t\tstack_canary2 = %ld\n", stack->canary1, stack->canary2);

    if (stack->data != (elem_t *) NOT_ALLOC_YET_PTR) {

        int64_t *data_canary1 = (int64_t *) ((char *)stack->data - sizeof(int64_t));
        int64_t *data_canary2 = (int64_t *) ((char *)stack->data + sizeof (elem_t) * stack->capacity);

        print_to_log ("\tData canaries: \n\t\tdata_canary1 = %ld \n\t\tdata_canary2 = %ld\n", *data_canary1, *data_canary2);
    }

    #endif

    #ifdef HASH

        print_to_log ("Stack hash = %ld\n", stack->stack_hash);
        print_to_log ("Data hash = %ld\n", stack->data_hash);


    #endif

    print_to_log ("}\n");

    print_to_log ("-----------------------------------------------------------------------------\n");

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
    if (errors != 0) {
        printf ("%d\n", errors);
    }

    if (stack->size < 0) {
        errors += STK_INV_SIZE;
    }
    if (errors != 0) {
        printf ("%d\n", errors);
    }

    if (stack->capacity < 0) {
        errors += STK_INV_CAPACITY;
    }
    if (errors != 0) {
        printf ("%d\n", errors);
    }

    if (stack->size > stack->capacity) {
        errors += STK_CAP_LESS_SIZE;
    }
    if (errors != 0) {
        printf ("%d\n", errors);
    }

    if (stack->size >= MAXCAPACITY || stack->capacity > MAXCAPACITY) {
        errors += STK_OVERFLOW;
    }
    if (errors != 0) {
        printf ("%d\n", errors);
    }

    if (stack->capacity > 0 && (stack->data == NULL || stack->data == (elem_t *) NOT_ALLOC_YET_PTR)) {
        errors += STK_INV_DATA_PTR;
    }
    if (errors != 0) {
        printf ("%d\n", errors);
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
            if (errors != 0) {
        printf ("%d\n", errors);
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

#ifdef HASH

int stack_struct_hash_check (Stack *stack)
{
    unsigned long struct_len = sizeof (Stack) - 2 * sizeof (int64_t);

    int64_t struct_hash = get_hash ((char *) stack, struct_len);

    if (struct_hash != stack->stack_hash) {
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

    unsigned long data_len = stack->capacity * sizeof (elem_t);

    int64_t data_hash = get_hash (stack->data, data_len);

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
        print_to_log ("Stack data hash protection was broken.\n");
    }

    return 0;
}

//------------------------------------------------------------------

int _print_error ( const char *file, const unsigned line, const char *func, const char *format, ...)
{
    assert (file);
    assert (func);
    assert (format);

    fprintf (stderr, "Error message from file %s, function %s (line %u): ", file, func, line);

    va_list args = {};

    va_start (args, format);
    vfprintf (stderr, format, args);

    va_end (args);

    fprintf (stderr, "\n");

    return 0;
    
}

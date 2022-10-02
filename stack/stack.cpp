#include "stack.h"
#include "../errors_and_logs/errors.h"
#include "../stack_hash/stack_hash.h"
#include "../config.h"

#include <stdlib.h>
#include <string.h>


int _stack_ctor (Stack *stack DEBUG_ARGS(, const char *func_name, const char *file, const unsigned int line, const char *func))  
{
    stack->data = (elem_t *) NOT_ALLOC_YET_PTR;

    stack->capacity = 0;
    stack->size = 0;
    stack->error_code = 0;

    #ifdef DEBUG

        #ifdef CANARIES

            stack->canary1 = CANARY_VALUE;
            stack->canary2 = CANARY_VALUE;

        #endif

        Var_info *info = stack->info;

        info->name = func_name;    
        info->function = func;
        info->file = file;
        info->line = line;
    
    #endif

    return 0;
}

//------------------------------------------------------------------

#ifdef CANARIES

int set_data_canaries (Stack *stack)
{
    int64_t *canary = (int64_t *)stack->data - 1;
    *canary = CANARY_VALUE;
    
    canary = (int64_t *) ((char *)stack->data + stack->capacity * sizeof(elem_t));
    *canary = CANARY_VALUE;

    return 0;
}

#endif

//------------------------------------------------------------------

int stack_push (Stack *stack, elem_t value)
{
    #ifdef DEBUG

        stack_verify(stack);
    
    #endif

    if (stack->capacity == 0 && stack->size == 0) {
        int set_data_ret = set_data (stack, START_STK_SIZE);
        if (set_data_ret ) return set_data_ret;
    }

    if (stack->size >= stack->capacity) {
        int resize_ret = stack_resize (stack, INCREASE);
        if (resize_ret) return resize_ret;
    }

    stack->data[stack->size++] = value;

    #ifdef DEBUG

        stack_get_data_hash (stack);

        stack_verify (stack);
    
    #endif

    return 0;
}

//------------------------------------------------------------------

elem_t stack_pop (Stack *stack) 
{   
    #ifdef DEBUG

        stack_pop_verify(stack);
    
    #endif

    elem_t value = stack->data[--stack->size];

    if (stack->capacity < stack->size * 4) {
        int resize_ret = stack_resize (stack, DECREASE);
        if (resize_ret) return resize_ret;
    }

    #ifdef DEBUG

        stack_get_data_hash (stack);

        stack_verify (stack);
    
    #endif

    return value;
}

//------------------------------------------------------------------

int set_data (Stack *stack, int size) 
{
    stack_verify(stack);

    if (size < 1) {
        /////////
    }

    #ifdef CANARIES

        elem_t *data_ptr = (elem_t *) calloc (size * sizeof(elem_t) + 2 * sizeof(int64_t), sizeof(char));

    #else

        elem_t *data_ptr = (elem_t *) calloc (size, sizeof(elem_t));

    #endif

    if (data_ptr == NULL) {
        ///
    }

    else {

        #ifdef CANARIES

            char *set_data_ptr = (char *) data_ptr + sizeof(int64_t);
            stack->data = (elem_t *) set_data_ptr;

            set_data_canaries(stack);

        #else 

            stack->data = data_ptr;

        #endif

    }

    return 0;
}

//------------------------------------------------------------------

int stack_resize (Stack *stack, int mode)
{
    #ifdef DEBUG

        stack_verify(stack);
    
    #endif

    if (stack->capacity == MAXCAPACITY && mode == INCREASE) {
        //error stackoverflow
    }

    size_t prev_capacity = stack->capacity;

    if (mode == INCREASE) {
        stack->capacity *= 2;
    }

    else if (mode == DECREASE) {
        stack->capacity /= 2;
    }

    else {
        ///err
    }

    if (stack->capacity > MAXCAPACITY) {
        stack->capacity = MAXCAPACITY;
    }

    #ifdef CANARIES

        char *canary1_ptr = (char *) stack->data - sizeof (int64_t);

        size_t size = stack->capacity * sizeof (elem_t) + sizeof (int64_t);
        size_t prev_size = prev_capacity * sizeof (elem_t) + 2 * sizeof (int64_t);

        char *new_data = (char *) my_recalloc (canary1_ptr, size, prev_size, sizeof (char));

    #else

        elem_t *new_data = (elem_t *) my_recalloc (stack->data, stack->capacity, prev_capacity, sizeof (elem_t));

    #endif

    if (new_data == NULL) {

        print_error (RECALLOC_ERR);
        return RECALLOC_ERR;
    
    }

    else {

        #ifdef CANARIES

            char *data_ptr = (char *) new_data + sizeof(int64_t);
            stack->data = (elem_t *) data_ptr;

            set_data_canaries (stack);

        #else 

            stack->data = new_data;

        #endif
    }

    #ifdef DEBUG

        stack_verify (stack);
    
    #endif

    return 0;
}

//------------------------------------------------------------------

int stack_dtor (Stack *stack)
{   
    free (stack->data);

    stack->data = (elem_t *) POISON_PTR;
    stack->capacity = POISON_VALUE;
    stack->size = POISON_VALUE;

    #ifdef DEBUG

        #ifdef CANARIES

            stack->canary1 = POISON_VALUE;
            stack->canary2 = POISON_VALUE;

        #endif

        Var_info *info = stack->info;

        info->name = POISON_NAME;
        info->function = POISON_NAME;
        info->file = POISON_NAME;
        info->line = POISON_VALUE;

        #ifdef HASH

            stack->stack_hash = POISON_VALUE;
            stack->data_hash  = POISON_VALUE;

        #endif

    #endif

    return 0;
}

//------------------------------------------------------------------

void *my_recalloc (void *ptr, size_t number, size_t prev_number, size_t elem_size) 
{
    if (ptr == NULL) {
        return NULL;
    }

    void *new_ptr = realloc (ptr, number * elem_size);

    if (new_ptr == NULL) {
        return NULL;
    }

    if (number > prev_number) {
        char *ptr_to_memset = (char *) new_ptr + prev_number * elem_size;

        memset (ptr_to_memset, 0, (number - prev_number) * elem_size);
    }

    return new_ptr;
}


//------------------------------------------------------------------
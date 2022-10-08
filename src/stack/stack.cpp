#include "stack.h"
#include "../errors_and_logs/errors.h"
#include "../stack_hash/stack_hash.h"
#include "../../config.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

//------------------------------------------------------------------

int var_info_ctor (Var_info *info, const char *var_name, const char *func, const char *file, unsigned int line)
{
    assert (info);

    info->name = var_name;    
    info->func = func;
    info->file = file;
    info->line = line;
    
    return 0;
} 

//------------------------------------------------------------------

int _stack_ctor (Stack *stack DEBUG_ARGS(, const char *var_name, const char *file, const unsigned int line, const char *func))  
{
    assert (stack);
    stack->data = (elem_t *) NOT_ALLOC_YET_PTR;

    stack->capacity = 0;
    stack->size = 0;
    stack->error_code = 0;
    
    #ifdef DEBUG

        #ifdef CANARIES

            stack->canary1 = CANARY_VALUE;
            stack->canary2 = CANARY_VALUE;

        #endif

        Var_info info = {};

        var_info_ctor (&info, var_name, func, file, line);

        stack->info = &info;

        #ifdef HASH

            int err = stack_update_struct_hash (stack);
            if (err < 0) return err;

        #endif
    
    #endif

    return 0;
}

//------------------------------------------------------------------

#ifdef CANARIES

static int _set_data_canaries (Stack *stack)
{
    int err = 0;

    #ifdef DEBUG

        err = stack_verify (stack);                
        if (err < 0) return err;

    #endif

    int64_t *canary_l = (int64_t *) stack->data - 1;
    *canary_l = CANARY_VALUE;
    
    int64_t *canary_r = (int64_t *) (stack->data + stack->capacity);
    *canary_r = CANARY_VALUE;

    #ifdef DEBUG

        #ifdef HASH

            err = stack_update_data_hash (stack);
            if (err < 0) return err;

        #endif

        err = stack_verify (stack);

        if (err < 0) return err;

    #endif



    return 0;
}

#endif

//------------------------------------------------------------------

static int _set_data (Stack *stack, int size) 
{
    int err = 0;


    #ifdef DEBUG

        err = stack_verify(stack);
        if (err < 0) return err;

    #endif

    if (size < 1) {
        print_error (SET_DATA_ERR);
        return SET_DATA_ERR;
    }

    #ifdef CANARIES

        elem_t *data_ptr = (elem_t *) calloc (size * sizeof(elem_t) + 2 * sizeof(int64_t), sizeof(char));
        // TODO проверять сразу 

    #else

        elem_t *data_ptr = (elem_t *) calloc (size, sizeof(elem_t));

    #endif

    if (data_ptr == NULL) {
        print_error (NULL_PTR_ERR);
        return NULL_PTR_ERR;
    }

    else {

        #ifdef CANARIES

            char *set_data_ptr = (char *) data_ptr + sizeof(int64_t);
            stack->data = (elem_t *) set_data_ptr;   ///////////////////////здесь живой

            _set_data_canaries(stack); ///здесь тоже

        #else 

            stack->data = data_ptr;

        #endif

    }

    #ifdef DEBUG

        #ifdef HASH

            err = stack_update_data_hash (stack);
            if (err < 0) return err;          ////жив
    
            err = stack_update_struct_hash (stack);   ///////////умер
            if (err < 0) return err;
        
        #endif

        err = stack_verify (stack);
        if (err < 0) return err;

    #endif

    return 0;
}

//------------------------------------------------------------------

static void *_my_recalloc (void *ptr, size_t number, size_t prev_number, size_t elem_size) 
{
    if (ptr == NULL) {
        return NULL;
    }

    void *new_ptr = realloc (ptr, number * elem_size);

    if (new_ptr == NULL) {
        return NULL;
    }

    if (number > prev_number) {
        char *poped_elem = (char *) new_ptr + prev_number * elem_size;

        memset (poped_elem, 0, (number - prev_number) * elem_size);
    }

    return new_ptr;
}

//------------------------------------------------------------------

static int _stack_resize (Stack *stack, int mode)
{
    int err = 0;

    #ifdef DEBUG

        if (stack->capacity == MAXCAPACITY && mode == INCREASE) {
            stack->error_code += STK_OVERFLOW;
        }

        err = stack_verify(stack);
        if (err < 0) return err;
    
    #endif


    if (mode == START_ALLOC) {
        err = _set_data (stack, START_STK_SIZE);
        if (err < 0) return err;
    }

    size_t prev_capacity = stack->capacity;

    switch (mode) {

        case INCREASE: {
            stack->capacity *= 2;
            break;
        }

        case DECREASE: {
            stack->capacity /= 2;
            break;
        }

        default: {
            print_error (RESIZE_MOD_ERR);
            return RESIZE_MOD_ERR;
        }
    }

    #ifdef CANARIES

        char *canary1_ptr = (char *) stack->data - sizeof (int64_t);

        size_t      size = stack->capacity * sizeof (elem_t) +     sizeof (int64_t);
        size_t prev_size =   prev_capacity * sizeof (elem_t) + 2 * sizeof (int64_t);

        char *new_data = (char *) _my_recalloc (canary1_ptr, size, prev_size, sizeof (char));


    #else

        elem_t *new_data = (elem_t *) _my_recalloc (stack->data, stack->capacity, prev_capacity, sizeof (elem_t));

    #endif

    if (new_data == NULL) {

        print_error (RECALLOC_ERR);
        return RECALLOC_ERR;
    
    }

    else {

        #ifdef CANARIES

            char *data_ptr = (char *) new_data + sizeof(int64_t);
            stack->data = (elem_t *) data_ptr;

            _set_data_canaries (stack);

        #else 

            stack->data = new_data;

        #endif
    }

    #ifdef DEBUG

        #ifdef HASH

            err = stack_update_data_hash (stack);
            if (err < 0) return err;

            err = stack_update_struct_hash (stack);
            if (err < 0) return err;

        #endif

        err = stack_verify (stack);
        if (err < 0) return err;

    #endif

    return 0;
}

//------------------------------------------------------------------

int stack_push (Stack *stack, elem_t value)
{
    int err = 0;
    #ifdef DEBUG

        err = stack_verify(stack);
        if (err < 0) return err;

    #endif

    if (stack->capacity == 0 && stack->size == 0) {
        err = _stack_resize (stack, START_ALLOC);
        if (err ) return err;
    }

    if (stack->size >= stack->capacity) {
        err = _stack_resize (stack, INCREASE);
        if (err) return err;              
    }

    stack->data[stack->size++] = value;

    #ifdef DEBUG

        #ifdef HASH

            err = stack_update_data_hash (stack);
            if (err < 0) return err;                
            
            err = stack_update_struct_hash (stack);
            if (err < 0) return err;

        #endif

        err = stack_verify (stack);
        if (err < 0) return err;

    #endif

    return 0;
}

//------------------------------------------------------------------

elem_t stack_pop (Stack *stack) 
{   
    int err = 0;

    #ifdef DEBUG

        if (stack->capacity == 0 || stack->size == 0) {
            stack->error_code += STK_POP_ERR;
        } 

        err = stack_verify (stack);
        if (err < 0) return 0;
    
    #endif

    elem_t value = stack->data[--stack->size];

    elem_t *poped_elem = stack->data + stack->size;

    if (memset (poped_elem, 0, sizeof (elem_t)) == NULL) {
        print_error (MEMSET_ERR);
        return MEMSET_ERR;
    }

    if (stack->capacity < stack->size * 4) {
        err = _stack_resize (stack, DECREASE);
        if (err) return err;
    }

    #ifdef DEBUG

        #ifdef HASH
            
            err = stack_update_data_hash (stack);
            if (err < 0) return err;
            
            err = stack_update_struct_hash (stack);
            if (err < 0) return err;

        #endif

        err = stack_verify (stack);
        if (err < 0) return err;

    #endif

    return value;
}

//------------------------------------------------------------------

int stack_dtor (Stack *stack)
{   
    int err = 0;

    #ifdef DEBUG

        err = stack_verify (stack);
        if (err < 0) return err;

    #endif
    
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
        info->func = POISON_NAME;  
        info->file = POISON_NAME;
        info->line = POISON_VALUE;

        #ifdef HASH

            stack->stack_hash = POISON_VALUE;
            stack->data_hash  = POISON_VALUE;

        #endif

    #endif

    return 0;
}
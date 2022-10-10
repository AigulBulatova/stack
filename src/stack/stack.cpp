#include "stack.h"
#include "../errors_and_logs/errors.h"
#include "../stack_hash/stack_hash.h"
#include "../../config.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

//------------------------------------------------------------------

#ifdef DEBUG

static int var_info_ctor (Var_info *info, const char *var_name, 
                          const char *func, const char *file, unsigned int line)
{
    assert (info);

    info->name = var_name;    
    info->func = func;
    info->file = file;
    info->line = line;
    
    return 0;
} 

#endif

//------------------------------------------------------------------

int _stack_ctor (Stack *stack DEBUG_ARGS(, const char *var_name, 
                const char *file, const unsigned int line, const char *func))  
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

        Var_info *info = (Var_info *) calloc (1, sizeof (Var_info));
        var_info_ctor (info, var_name, func, file, line);

        stack->info = info;

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

    int64_t *canary_l = (int64_t *) stack->data - 1;
    *canary_l = CANARY_VALUE;
    
    int64_t *canary_r = (int64_t *) (stack->data + stack->capacity);
    *canary_r = CANARY_VALUE;
    printf ("%p  %p  %p\n", canary_l, stack->data, canary_r);   

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

    if (size < 1) {
        print_error ("Set data error: cannot set size of data %d", size);
        return SET_DATA_ERR;
    }

    stack->capacity = START_STK_SIZE;

    #ifdef CANARIES

        elem_t *data_ptr = (elem_t *) calloc ((size_t) size * sizeof(elem_t) + 2 * sizeof(int64_t), sizeof(char));

    #else

        elem_t *data_ptr = (elem_t *) calloc (size, sizeof(elem_t));

    #endif

    if (data_ptr == NULL) {
        print_error ("Can not allocate memory.");          
        return NO_MEMORY_ERR;
    }

    else {

        #ifdef CANARIES

            char *set_data_ptr = (char *) data_ptr + sizeof(int64_t);
            stack->data = (elem_t *) set_data_ptr;   

            _set_data_canaries(stack); 

        #else 

            stack->data = data_ptr;

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

static int _stack_resize (Stack *stack, Modes mode)
{
    int err = 0;

    if (stack->capacity == MAXCAPACITY && mode == INCREASE) {
        print_to_log ("Stack overflow");
        return -1;            
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

        case START_ALLOC: {
            err = _set_data (stack, START_STK_SIZE); 
            if (err < 0) return err;
            break;
        }

        default: {
            print_error ("Unexpected resize mod: %d", mode);
            return RESIZE_MOD_ERR;
        }
    }

    #ifdef CANARIES

        char *canary1_ptr = (char *) stack->data - sizeof (int64_t);

        size_t      size = stack->capacity * sizeof (elem_t) + 2 * sizeof (int64_t);
        size_t prev_size =   prev_capacity * sizeof (elem_t) + 2 * sizeof (int64_t);

        char *new_data = (char *) _my_recalloc (canary1_ptr, size, prev_size, sizeof (char));

    #else

        size_t      size = stack->capacity * sizeof (elem_t);
        size_t prev_size =   prev_capacity * sizeof (elem_t);

        char *new_data = (char *) _my_recalloc (stack->data, stack->capacity, prev_capacity, sizeof (elem_t));

    #endif

    if (new_data == NULL) {

        print_error ("Can not allocate memory with _my_recalloc().");
        return RECALLOC_ERR;
    
    }

    else {

        #ifdef CANARIES

            char *data_ptr = new_data + sizeof(int64_t);
            stack->data = (elem_t *) data_ptr;

            _set_data_canaries (stack);

        #else 

            stack->data = (elem_t *) new_data;

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
    printf ("%lu  %lu\n", stack->capacity, stack->size);

    stack->data[stack->size++] = value;

    #ifdef DEBUG

        #ifdef HASH

            err = stack_update_data_hash (stack);
            if (err < 0) return err;             
            
            err = stack_update_struct_hash (stack);
            if (err < 0) return err;

        #endif

        err = stack_verify (stack);          ///umer
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
        print_error ("Memset error.");
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

    #ifdef CANARIES

        char *data = (char *) stack->data - sizeof(int64_t);

    #else 

        char *data = (char *) stack->data;

    #endif


    free (data);

    stack->data = (elem_t *) POISON_PTR;
    stack->capacity = POISON_VALUE;
    stack->size = POISON_VALUE;

    #ifdef DEBUG

        stack->error_code = 0;
        #ifdef CANARIES

            stack->canary1 = POISON_VALUE;
            stack->canary2 = POISON_VALUE;

        #endif

        Var_info *info = stack->info;

        info->name = "POISONED";
        info->func = "POISONED";  
        info->file = "POISONED";
        info->line = POISON_VALUE;

        free (stack->info);

        #ifdef HASH

            stack->stack_hash = POISON_VALUE;
            stack->data_hash  = POISON_VALUE;

        #endif

    #endif

    return 0;
}
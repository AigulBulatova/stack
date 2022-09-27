#include "stack.h"
#include "../config.h"


int _stack_ctor (Stack *stack, const char *func_name, const char *file, const unsigned int line, const char *func)  
{
    stack->data = (elem_t *) DEFAULT_PTR;

    stack->capacity = 0;
    stack->size = 0;

    #ifdef CANARIES

        stack->canary1 = CANARY;
        stack->canary2 = CANARY;

    #endif

    Stack_info *info = stack->info;

    info->name = func_name;    
    info->function = func;
    info->file = file;
    info->line = line;

}

//------------------------------------------------------------------

#ifdef CANARIES

int set_data_canaries (Stack *stack)
{
    int64_t *canary = (int64_t *)stack->data - 1;
    *canary = CANARY;
    
    canary = (int64_t *) ((char *)stack->data + stack->capacity * sizeof(elem_t));
    *canary = CANARY;

    return 0;
}

#endif

//------------------------------------------------------------------

int stack_push (Stack *stack, elem_t value)
{
    stack_verify(stack);

    if (stack->capacity == 0 && stack->size == 0) {
        int set_data_ret = set_data (stack, START_STK_SIZE);
        if (set_data_ret ) return set_data_ret;
    }

    if (stack->size >= stack->capacity) {
        int resize_ret = stack_resize (stack, INCREASE);
        if (resize_ret) return resize_ret;
    }

    stack->data[stack->size++] = value;

    return 0;
}

//------------------------------------------------------------------

elem_t stack_pop (Stack *stack) 
{
    stack_verify_pop(stack);

    if (stack->size == 0 || stack->capacity == 0) {
        ///нет элементов
    }

    elem_t value = stack->data[--stack->size];

    if (stack->capacity == stack->size * 4) {
        int resize_ret = stack_resize (stack, DECREASE);
        if (resize_ret) return resize_ret;
    }

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
    stack_verify(stack);

    if (stack->capacity == MAXCAPACITY && mode == INCREASE) {
        //error stackoverflow
    }

    if (mode == INCREASE) {
        stack->capacity *= 2;
    }

    else if (mode == DECREASE) {
        stack->capacity \= 2;
    }

    else {
        ///err
    }

    if (stack->capacity > MAXCAPACITY) {
        stack->capacity = MAXCAPACITY;
    }

    #ifdef CANARIES

        char *canary1_ptr = (char *) stack->data - sizeof(int64_t);
        elem_t *new_data = (elem_t *) realloc (canary1_ptr, stack->capacity * sizeof(elem_t) + 2 * sizeof(int64_t));

    #else

        elem_t *new_data = (elem_t *) realloc (stack->data, stack->capacity * sizeof(elem_t));

    #endif

    if (new_data == NULL) {
        //////
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

    return 0;
}

//------------------------------------------------------------------

int stack_dtor (Stack *stack)
{
    stack_verify(stack);

    free (stack->data);

    stack->data = POISON_PTR;

    #ifdef CANARIES
        stack->canary1 = POISON_VALUE;
        stack->canary2 = POISON_VALUE;

    #endif

    stack->capacity = -1;
    stack->size = -1;

    Var_info *info = stack->info;

    info->name = POISON_NAME;
    info->function = POISON_NAME;
    info->file = POISON_NAME;

    info->line = -1;

    return 0;
}

//------------------------------------------------------------------

unsigned int get_hash (char * key, unsigned int len)
{
    const unsigned int m = 0x5bd1e995;
    const unsigned int seed = 0;
    const int r = 24;

    unsigned int h = seed ^ len;

    const unsigned char * data = (const unsigned char *) key;
    unsigned int k = 0;

    while (len >= 4)
    {
        k  = data[0];
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch (len)
    {
        case 3: {
            h ^= data[2] << 16;
        }

        case 2: {
            h ^= data[1] << 8;
        }
        
        case 1: {
            h ^= data[0];
            h *= m;
        }
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}

//------------------------------------------------------------------

#ifdef HASH

int set_hash (Stack *stack, unsigned int hash)
{
    stack_verify(stack);


    stack->stack_hash = get_hash (stack, sizeof (Stack));
    stack->data_hash = get_hash (stack->data, stack->capacity); 

    return 0;
}

#endif
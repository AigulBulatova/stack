#include "stack_hash.h"
#include "../stack/stack.h"
#include <stdio.h>

//------------------------------------------------------------------

typedef int64_t (*hash_func) (void *base, unsigned int len);

static hash_func stack_hash_func = DEFAULT_HASH_FUNC;

//------------------------------------------------------------------

#ifdef HASH 

int64_t get_hash (void *base, unsigned int len) 
{
    const unsigned int m = 0x5bd1e995;
    const unsigned int seed = 0;
    const int r = 24;

    unsigned int h = seed ^ len;

    const unsigned char *data = (const unsigned char *) base;
    assert (base);

    unsigned int k = 0;

    while (len >= 4) {

        k  = data[0];
        k |= (data[1] << 8);
        k |= (data[2] << 16);
        k |= (data[3] << 24);

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len  -= 4;

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

#endif

//------------------------------------------------------------------

#ifdef HASH 

int stack_get_struct_hash (Stack *stack)
{
    stack_verify (stack);

    unsigned int len = sizeof (Stack) - 2 * sizeof (int64_t);

    

}

#endif

//------------------------------------------------------------------

#ifdef HASH 

int stack_get_data_hash (Stack *stack)
{
    stack_verify (stack);

    #ifdef CANARIES

        char *data = (char *) stack->data - sizeof (int64_t);
        unsigned int len = stack->capacity * sizeof (elem_t) + 2 * sizeof (int64_t);

    #else 

        char *data = (char *) stack->data;
        unsigned int len = stack->capacity * sizeof (elem_t);

    #endif

    stack->data_hash = stack_hash_func (data, len);

    return 0;
}

#endif

//------------------------------------------------------------------

#ifdef HASH

int stack_hash_func (int64_t (*get_hash_func) (void *base, unsigned int len))
{
    if (get_hash_func == NULL) {
        /////err
    }

    stack_hash_func = get_hash_func;

    return 0;
}

#endif
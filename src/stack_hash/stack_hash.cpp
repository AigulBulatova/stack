#include "stack_hash.h"
#include "../stack/stack.h"
#include "../errors_and_logs/errors.h"
#include "../../config.h"

#include <stdio.h>
#include <assert.h>

//------------------------------------------------------------------

typedef int64_t (*hash_func) (void *base, unsigned long len);

static hash_func stack_hash_func = DEFAULT_HASH_FUNC;

//------------------------------------------------------------------

#ifdef HASH 

int64_t get_hash (void *base, unsigned long len) 
{
    const unsigned int m = 0x5bd1e995;
    const unsigned int seed = 0;
    const int r = 24;

    unsigned long h = seed ^ len;

    const unsigned char *data = (const unsigned char *) base;
    assert (base);

    unsigned int k = 0;

    while (len >= 4) {

        k  = (unsigned) data[0];
        k |= (unsigned) (data[1] << 8);
        k |= (unsigned) (data[2] << 16);
        k |= (unsigned) (data[3] << 24);

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
            break;
        }

        case 2: {
            h ^= data[1] << 8;
            break;
        }
        
        case 1: {
            h ^= data[0];
            h *= m;
            break;
        }
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return  (unsigned) h;
}

#endif

//------------------------------------------------------------------

#ifdef HASH 

int stack_update_struct_hash (Stack *stack)
{
    assert (stack);
    // if (stack->stack_hash != 0) {
    //     stack_verify (stack);             /////////////////////////////////застревает здесь
    // }


    unsigned long len = sizeof (Stack) - 2 * sizeof (int64_t);

    stack->stack_hash = stack_hash_func (stack, len);

    return 0; 
}

#endif

//------------------------------------------------------------------

#ifdef HASH 

int stack_update_data_hash (Stack *stack)
{
    assert (stack);

    stack_verify (stack);

    #ifdef CANARIES

        char *data = (char *) stack->data - sizeof (int64_t);
        unsigned long len = stack->capacity * sizeof (elem_t) + 2 * sizeof (int64_t);

    #else 

        char *data = (char *) stack->data;
        unsigned long len = stack->capacity * sizeof (elem_t);

    #endif

    stack->data_hash = stack_hash_func (data, len);

    return 0;
}

#endif

//------------------------------------------------------------------

#ifdef HASH

int set_stack_hash_func (int64_t (*get_hash_func) (void *base, unsigned long len))
{
    if (get_hash_func == NULL) {
        print_error (NULL_PTR_ERR);
        return NULL_PTR_ERR;
    }

    stack_hash_func = get_hash_func;

    return 0;
}

#endif
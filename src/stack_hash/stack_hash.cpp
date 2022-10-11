#include <stdio.h>
#include <assert.h>

#include "stack_hash.h"
#include "../stack/stack.h"
#include "../errors_and_logs/errors.h"
#include "../../config.h"
#include "../general/general.h"

//------------------------------------------------------------------

typedef int64_t (*hash_func) (void *base, unsigned long len);

#ifdef HASH

static hash_func stack_hash_func = DEFAULT_HASH_FUNC;

#endif

//------------------------------------------------------------------

#ifdef HASH 

int stack_update_struct_hash (Stack *stack)
{
    assert (stack);

    unsigned long len = sizeof (Stack) - 2 * sizeof (int64_t);

    stack->stack_hash = stack_hash_func ((char *) stack, len);

    return 0; 
}

#endif

//------------------------------------------------------------------

#ifdef HASH 

int stack_update_data_hash (Stack *stack)
{
    assert (stack);

    unsigned long len = stack->capacity * sizeof (elem_t);

    stack->data_hash = stack_hash_func (stack->data, len);

    return 0;
}

#endif

//------------------------------------------------------------------

#ifdef HASH

int set_stack_hash_func (int64_t (*get_hash_func) (void *base, unsigned long len))
{
    if (get_hash_func == NULL) {
        print_error ("Null pointer to hash function.");
        return NULL_PTR_ERR;
    }

    stack_hash_func = get_hash_func;

    return 0;
}

#endif
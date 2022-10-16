#pragma once


#include "../stack/stack.h"

//------------------------------------------------------------------

enum stack_errors {
    STK_STRUCT_NULL_PTR    = 0x1,
    STK_INV_CAPACITY       = 0x2,
    STK_INV_SIZE           = 0x4,
    STK_CAP_LESS_SIZE      = 0x8,
    STK_STRUCT_CANARY1_ERR = 0x10,
    STK_STRUCT_CANARY2_ERR = 0x20,
    STK_OVERFLOW           = 0x40,
    STK_DATA_NULL_PTR      = 0x80,
    STK_POP_ERR            = 0x100,
    STK_DATA_CANARY1_ERR   = 0x200,
    STK_DATA_CANARY2_ERR   = 0x400,
    STK_STRUCT_HASH_ERR    = 0x800,
    STK_DATA_HASH_ERR      = 0x1000,
    STK_INV_DATA_PTR       = 0x2000,
    STK_SELF_PTR_ERR       = 0x4000,
};

//------------------------------------------------------------------

#define stack_verify(stack) \
        _stack_verify (stack, LOCATION)

#define stack_pop_verify(stack) \
        _stack_pop_verify (stack, LOCATION)

//------------------------------------------------------------------

int stack_update_struct_hash (Stack *stack);

int stack_update_data_hash   (Stack *stack);

int set_stack_hash_func (int64_t (*get_hash_func) (void *base, unsigned long len));

int _set_data_canaries (Stack *stack);

int stack_struct_hash_check (Stack *stack);

int stack_data_hash_check (Stack *stack);

int stack_canary_verify (Stack *stack);

int _stack_verify (Stack *stack, const char *file, const unsigned line, const char *function);

int _stack_dump (Stack *stack, const char *file, const unsigned line, const char *func);

int print_stack_error (int errors);


#pragma once

#include "logs.h"
#include "../stack/stack.h"

//------------------------------------------------------------------

enum stack_rrors {
    STK_STRUCT_NULL_PTR = 0x1,
    STK_INV_CAPACITY    = 0x2,
    STK_INV_SIZE        = 0x4,
    STK_CAP_LESS_SIZE   = 0x8,
    STK_CANARY_ERR      = 0x10,
    STK_OVERFLOW        = 0x20,
    STK_SET_DATA_ERR    = 0x40,
    STK_DATA_NULL_PTR   = 0x80,
    STK_RESIZE_MOD_ERR  = 0x100,
    STK_STRUCT_CANARY_ERR =0x200,
    STK_DATA_CANARY_ERR  = 0x400,
    STK_STRUCK_HASH_ERR  = 0x800,
    STK_DATA_HASH_ERR    = 0x1000,
    STK_POP_ERR,
};

enum general_errors {
    OPEN_FILE_ERR  = -2,
    CLOSE_FILE_ERR = -3,
    NULL_PTR_ERR   = -4,
    NO_MEMORY_ERR  = -5,
    RECALLOC_ERR   = -6,

};

//------------------------------------------------------------------

#define stack_verify(stack) \
        if (_stack_verify (stack, LOCATION) == -1) {    \
            return -1;                                      \
        }

#define stack_pop_verify(stack) \
        if (_stack_pop_verify (stack, LOCATION) == -1) {    \
            return -1;                                      \
        }

#define print_error(error) \
        _print_error(error, stderr, LOCATION)

//------------------------------------------------------------------

int _stack_verify (Stack *stack, const char *file, const unsigned line, const char *function);

int _stack_pop_verify (Stack *stack, const char *file, const unsigned line, const char *function);

int stack_struct_hash_chack (Stack *stack);

int stack_data_hash_check (Stack *stack);

int print_stack_error (int errors);

int _print_error (int error, FILE *err_file, const char *file, const unsigned line, const char *func);

int _stack_dump (Stack *stack, const char *file, const unsigned line, const char *func);
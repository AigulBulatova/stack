#pragma once

#include "logs.h"

//------------------------------------------------------------------

enum stack_rrors {
    STK_NULL_PTR      = 0x1,
    STK_INV_CAPACITY  = 0x2,
    STK_INV_SIZE      = 0x4,
    STK_CAP_LESS_SIZE = 0x8,
    STK_CANARY_ERR    = 0x10,
    STK_OVERFLOW      = 0x20,


};

enum general_errors {
    OPEN_FILE_ERR  = -2;
    CLOSE_FILE_ERR = -3;
    NULL_PTR_ERR   = -4;
    NO_MEMORY_ERR  = -5;

}

//------------------------------------------------------------------

#define stack_verify(stack) \
        _stack_verify(stack, LOCATION)

#define stack_pop_verify(stack) \
        _stack_pop+stack_verify(stack, LOCATION)



//------------------------------------------------------------------

int _stack_verify (Stack *stack);

int stack_canary_verify (Stack *stack);

int print_stack_error (int error);

int _stack_dump (Stack *stack, const char *file, const unsigned line, const char *func);
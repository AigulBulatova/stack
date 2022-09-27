#pragma once

//------------------------------------------------------------------

enum Errors {
        STK_NULL_PTR      = 1,
        STK_INV_CAPACITY  = 2,
        STK_INV_SIZE      = 4,
        STK_CAP_LESS_SIZE = 8,
        STK_CANARY_ERR    = 16,
        STK_OVERFLOW      = 32,

};

//------------------------------------------------------------------

#define stack_dump(stack, errors) \
        _stack_dump(stack, errors,  LOCATION)
        
#define ASSERT(errors, stack) \
        if (errors != 0) stack_dump(stack, errors)

#define stack_verify_pop(stack) \
        int errors = 0;                         \
        if (stack->capacity == 0) {             \
                errors += STK_INV_CAPACITY;     \
        }                                       \
        errors += _stack_verify (stack);        \
        ASSERT(errors, stack)

#define stack_verify(stack) \
        int errors = _stack_verify(stack);      \
        ASSERT (errors, stack)

//------------------------------------------------------------------

int _stack_verify (Stack *stack);

int stack_canary_verify (Stack *stack);

int print_error (int error);

void _stack_dump (Stack *stack, const char *file, const unsigned int line, const char *func);
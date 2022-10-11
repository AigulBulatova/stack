#pragma once

//------------------------------------------------------------------
#include <stdio.h>

#include "../../config.h"

//------------------------------------------------------------------

#ifdef DEBUG

    #define DEBUG_ARGS(...) __VA_ARGS__

#else

    #define DEBUG_ARGS(...)

#endif

//------------------------------------------------------------------

enum Modes {
    INCREASE    = 1,
    DECREASE    = 2,
    START_ALLOC = 3,
};

//------------------------------------------------------------------

struct Var_info {

    const char *name;
    const char *func;
    const char *file;
    unsigned line;

};

struct Stack {
    #ifdef DEBUG

        #ifdef CANARIES
            int64_t canary1;
        #endif

    #endif

    elem_t *data;
    size_t size;
    size_t capacity;

    #ifdef DEBUG

        Var_info *info;
        Stack *self_prt;

        int error_code;

        #ifdef CANARIES
            int64_t canary2;
        #endif

        #ifdef HASH
            int64_t stack_hash;
            int64_t data_hash;
        #endif

    #endif

};

#define LOCATION \
        __FILE__, __LINE__, __FUNCTION__

//------------------------------------------------------------------

#define stack_ctor(stack) \
        _stack_ctor(stack DEBUG_ARGS(, #stack + 1, LOCATION))

//------------------------------------------------------------------

int _stack_ctor (Stack *stack DEBUG_ARGS(, const char *func_name, const char *file, const unsigned int line, const char *func));

int stack_push (Stack *stack, elem_t value);

elem_t stack_pop (Stack *stack);

int stack_dtor (Stack *stack);



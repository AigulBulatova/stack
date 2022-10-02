#pragma once

//------------------------------------------------------------------

#include "../config.h"
#include <stdio.h>

//------------------------------------------------------------------

#ifdef DEBUG

    #define DEBUG_ARGS(...) __VA_ARGS__

#else

    #define DEBUG_ARGS(...)

#endif

//------------------------------------------------------------------

enum Modes {
    INCREASE = 1,
    DECREASE = 2
};

//------------------------------------------------------------------

struct Var_info {

    const char *name;
    const char *function;
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

int set_data_canaries (Stack *stack);

int stack_push (Stack *stack, elem_t value);

elem_t stack_pop (Stack *stack);

int set_data (Stack *stack, int size);

int stack_resize (Stack *stack, int mode);

int stack_dtor (Stack *stack);

void *my_recalloc (void *ptr, size_t number, size_t prev_number, size_t elem_size);









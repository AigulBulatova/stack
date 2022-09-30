#pragma once

//------------------------------------------------------------------

#include "../config.h"
#include <stdio.h>

//------------------------------------------------------------------

#define MAXCAPACITY 100

#define NOT_ALLOC_YET_PTR 435

#define POISON_PTR 13

#define CANARY_VALUE 0x

#define START_STK_SIZE 2

#define POISON_VALUE 228

#define POISON_NAME "POISON"

#ifdef CANARY
#define ON_CANARY_PROT(...) __VA_ARGS__
#else
#define ON_CANARY_PROT(...)
#endif

//------------------------------------------------------------------

enum Modes {
    INCREASE = 1,
    DECREASE = 2
};

//------------------------------------------------------------------

struct Var_info {

    char *name;
    char *function;
    char *file;
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
            int64_t base_hash;
        #endif

    #endif

};

#define LOCATION \
        __FILE__, __LINE__, __FUNCTION__

//------------------------------------------------------------------

#define stack_ctor(stack) \
        _stack_ctor(stack, #stack + 1, LOCATION)

//------------------------------------------------------------------

int _stack_ctor  (Stack *stack, const char *func_name, const char *file, const unsigned int line, const char *func);

int stack_push   (Stack *stack, elem_t value);

elem_t stack_pop (Stack *stack);

int stack_dtor   (Stack *stack);

int stack_resize (Stack *stack, int mode);

int set_data_canaries (Stack *stack);

int set_data (Stack *stack, int size);







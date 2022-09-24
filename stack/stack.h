#pragma once

//------------------------------------------------------------------

struct Info {
    char *name;
    char *function;
    char *file;
    unsigned line;
};

struct Stack {
    double *data;
    size_t size;
    size_t capacity;

    Info *info;
};

//------------------------------------------------------------------

#define stack_ctor(stack) \
        _stack_ctor(stack, __FILE__, __LINE__, __FUNCTION__)

//------------------------------------------------------------------

int _stack_ctor (Stack *stack, const char *file, const unsigned int line, const char *func);

int stack_push (Stack *stack, double value);

double stack_pop (Stack *stack);

int stack_dtor (Stack *stack);




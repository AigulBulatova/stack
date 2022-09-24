#pragma once

//------------------------------------------------------------------

#define stack_dump(stack) \
        _stack_dump(stack, __FILE__, __LINE__, __FUNCTION__)

//------------------------------------------------------------------

int stack_verify (Stack *stack);

int print_error (int error);

void _stack_dump (Stack *stack, const char *file, const unsigned int line, const char *func);
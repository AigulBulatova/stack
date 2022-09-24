#include "stack.h"


int _stack_ctor (Stack *stack, const char *file, const unsigned int line, const char *func)   ///имя????????
{
    assert (stack); /////
    assert (file);
    assert (line);

    Info *info = stack->info;
    //info->name =         ??????
    info->function = func;
    info->file = file;
    info->line = line;

    stack->data = NULL;
    stack->size = 0;
    stack->capacity = 0;

}

int stack_push (Stack *stack, double value)
{
    assert (!stack_verify (stack));

    if ()
}
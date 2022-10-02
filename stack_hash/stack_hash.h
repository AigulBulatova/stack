#pragma once

#include "../stack/stack.h"

//------------------------------------------------------------------

int stack_get_struct_hash (Stack *stack);

int stack_get_data_hash   (Stack *stack);

int stack_hash_func (int64_t (*get_hash_func) (void *base, unsigned int len));

int64_t get_hash (void *base, unsigned int len);
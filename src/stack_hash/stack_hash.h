#pragma once

#include "../stack/stack.h"

//------------------------------------------------------------------

int stack_update_struct_hash (Stack *stack);

int stack_update_data_hash   (Stack *stack);

int set_stack_hash_func (int64_t (*get_hash_func) (void *base, unsigned long len)); 

int64_t get_hash (void *base, unsigned long len); // TODO utilities
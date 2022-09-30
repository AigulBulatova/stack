#pragma once

//------------------------------------------------------------------

int stack_get_struct_hash (Stack *stack);

int stack_get_data_hash   (Stack *stack);

int stack_hash_func       (Stack *stack);

unsigned int get_hash     (char * key, unsigned int len);
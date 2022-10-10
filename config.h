#pragma once //TODO stackconf

#include <stdint.h>
//---------------------------------------------------------

//Typedef for type of stack elements
typedef double elem_t;

//Specifier for printing stack elements
#define TYPE_SPEC "%lf" //TODO -> static const char* 

//---------------------------------------------------------

//Turns on debug mode
#define DEBUG

//Turns on canary protection
#define CANARIES

//Turns on hash protection
#define HASH

//---------------------------------------------------------

//Default pointer to hash function
#define DEFAULT_HASH_FUNC &get_hash

//Pointer to stack->data before the first stack_push() call.
const static int NOT_ALLOC_YET_PTR = 435;

//Maximum capacity of the stack.
const static int MAXCAPACITY = 100;

//Poisoned pointer for stack->data to set in stack_dtor().
const static int POISON_PTR = 13;

//Poisoned value for stack fields to set in stack_dtor(). 
const static int POISON_VALUE = 228;

//Number of stack elements to allocate before first pushing. 
const static int START_STK_SIZE = 2;

//Default canary value for canary protection.
const static int64_t CANARY_VALUE = 0xDEADDEAD;
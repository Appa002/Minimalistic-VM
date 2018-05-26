//
// Created by a_mod on 10.05.2018.
//

#ifndef VIRTUAL_MACHIEN_STACK_H
#define VIRTUAL_MACHIEN_STACK_H

#include <stdint.h>
#include "object.h"

typedef struct Stack{
    object_t* data;
    uint8_t top;
} stack_t;

//#define pop_stack(type, stack)  ({type var; var = read_from_object(type, stack); remove_from_stack(stack); var;})

object_t pop_stack(stack_t* stack);

object_t peek_stack(stack_t* stack);

void push_stack(stack_t* stack, object_t object);

stack_t new_stack(uint16_t size);

void del_stack(stack_t* stack);

#endif //VIRTUAL_MACHIEN_STACK_H

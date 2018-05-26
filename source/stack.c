//
// Created by a_mod on 26.05.2018.
//

#include <stdlib.h>

#include "../headers/stack.h"

object_t pop_stack(stack_t* stack){
    return stack->data[stack->top--];
}

object_t peek_stack(stack_t* stack){
    return stack->data[stack->top];
}

void push_stack(stack_t* stack, object_t object){
    stack->data[++stack->top] = object;
}

stack_t new_stack(uint16_t size){
    stack_t s;
    s.data = (object_t*)malloc(size * sizeof(object_t));
    s.top = 0;
    return s;
}

void del_stack(stack_t* stack){
    free(stack->data);
}
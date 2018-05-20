//
// Created by a_mod on 10.05.2018.
//

#ifndef VIRTUAL_MACHIEN_STACK_H
#define VIRTUAL_MACHIEN_STACK_H

#include <stdint.h>

enum Object_Types  {
    OBJECT_FLAG,
    OBJECT_SIGNED,
    OBJECT_UNSIGNED,
    OBJECT_FLOAT,
    OBJECT_POINTER
};

typedef struct Object{
    enum Object_Types type;
    size_t size;
    void* data;
} object_t;

typedef struct Stack{
    object_t* data;
    uint8_t top;
} stack_t;

#define pop_stack(type, stack)  ({type var; var = *(type *) peek_stack(stack).data; remove_from_stack(stack); var;})

void del_stack(stack_t* stack);

object_t peek_stack(stack_t* stack){
    return stack->data[stack->top];
}

void remove_from_stack(stack_t *stack){
    free(stack->data[stack->top--].data);
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
    while (stack->top > 0){
        remove_from_stack(stack);
    }
    free(stack->data);
}

#endif //VIRTUAL_MACHIEN_STACK_H

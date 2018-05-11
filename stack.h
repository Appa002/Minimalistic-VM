//
// Created by a_mod on 10.05.2018.
//

#ifndef VIRTUAL_MACHIEN_STACK_H
#define VIRTUAL_MACHIEN_STACK_H

#include <stdint.h>

enum Object_Types  {
    OBJECT_FLAG,
    OBJECT_CHAR,
    OBJECT_UNSIGNED_8,
    OBJECT_SIGNED_8,
    OBJECT_UNSIGNED_16,
    OBJECT_SIGNED_16,
    OBJECT_UNSIGNED_32,
    OBJECT_SIGNED_32,
    OBJECT_FLOAT,
    OBJECT_POINTER
};

typedef struct Object{
    enum Object_Types type;
    union {
        uint8_t ui8;
        uint16_t ui16;
        uint32_t ui32;

        int8_t i8;
        int16_t i16;
        int32_t i32;

        float f;

        void* ptr;
    };
} object_t;

typedef struct Stack{
    object_t* data;
    uint8_t top;
} stack_t;

stack_t new_stack(uint16_t size){
    stack_t s;
    s.data = (object_t*)malloc(size * sizeof(object_t));
    s.top = 0;
    return s;
}

void del_stack(stack_t* stack){
    free(stack->data);
}

object_t peek_stack(stack_t* stack){
    return stack->data[stack->top];
}

object_t pop_stack(stack_t* stack){
    return stack->data[stack->top--];
}

void push_stack(stack_t* stack, object_t object){
    stack->data[++stack->top] = object;
}


#endif //VIRTUAL_MACHIEN_STACK_H

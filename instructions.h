//
// Created by a_mod on 10.05.2018.
//

#ifndef VIRTUAL_MACHIEN_INSTRUCTIONS_H
#define VIRTUAL_MACHIEN_INSTRUCTIONS_H

#include "stack.h"

typedef uint8_t* (*instruction)(uint8_t * ip, stack_t* stack);


uint8_t* opt_nop(uint8_t* ip, stack_t* stack){
    return ip + 1;
}

uint8_t* opt_push_byte(uint8_t *ip, stack_t *stack){
    object_t o;
    o.type = OBJECT_UNSIGNED_8;
    o.ui8 = *(ip + 1);
    push_stack(stack, o);
    return ip + 2;
}

uint8_t* opt_emit(uint8_t* ip, stack_t* stack) {
    object_t o = pop_stack(stack);
    char data[2];

    switch (o.type){
        case(OBJECT_FLOAT):
            printf("%.6f", o.f);
            break;
        case(OBJECT_UNSIGNED_8):
            printf("%u", o.ui8);
            break;
        case(OBJECT_UNSIGNED_16):
            printf("%u", o.ui16);
            break;
        case (OBJECT_UNSIGNED_32):
            printf("%u", o.ui32);
            break;
        case(OBJECT_SIGNED_8):
            printf("%d", o.ui8);
            break;
        case(OBJECT_SIGNED_16):
            printf("%d", o.ui16);
            break;
        case (OBJECT_SIGNED_32):
            printf("%d", o.ui32);
            break;
        default:
            break;
    }
    return ip + 1;
}

uint8_t* opt_emit_char(uint8_t* ip, stack_t* stack){
    if(peek_stack(stack).type != OBJECT_UNSIGNED_8)
        exit(-10);

    printf("%c", pop_stack(stack).ui8);
    return ip + 1;
}

uint8_t* opt_jump(uint8_t* ip, stack_t* stack){
    return stack->data[0].ptr + *(ip + 1);
}

uint8_t* opt_push_string(uint8_t* ip, stack_t* stack){
    uint8_t length_of_string = *(ip + 1);
    for(size_t i = 0; i < length_of_string; i++){
        object_t o;
        o.type = OBJECT_UNSIGNED_8;
        o.ui8 = *(ip + i + 2);
        push_stack(stack, o);
    }

    return ip + 2 + length_of_string;
}

uint8_t* opt_emit_string(uint8_t* ip, stack_t* stack) {
    uint8_t length_of_string = *(ip + 1);
    for (size_t i = 0; i < length_of_string; i++) {
        if (peek_stack(stack).type != OBJECT_UNSIGNED_8) {
            exit(-10);
        }

        char data[2];
        data[0] = pop_stack(stack).ui8;
        data[1] = 0;
        printf(data);
    }

    return ip + 2;
}

uint8_t* opt_compare(uint8_t* ip, stack_t* stack) {
    uint8_t first = *(ip+1);
    uint8_t second = *(ip+2);

    object_t object;
    if(first-second == 0){
        object.type = OBJECT_FLAG;
        object.ui8 = 0;
    }else if(first-second > 0) {
        object.type = OBJECT_FLAG;
        object.ui8 = 1;
    }else{
        object.type = OBJECT_FLAG;
        object.ui8 = 2;
    }
    push_stack(stack, object);

    return ip + 3;
}

uint8_t* opt_jump_not_equal(uint8_t* ip, stack_t* stack) {
    if(peek_stack(stack).type != OBJECT_FLAG)
        exit(-10);

    uint8_t flag = pop_stack(stack).ui8;
    if(flag != 0)
        return stack->data[0].ptr + *(ip + 1);
    else
        return ip + 2;
}

uint8_t* opt_jump_equal(uint8_t* ip, stack_t* stack) {
    if(peek_stack(stack).type != OBJECT_FLAG)
        exit(-10);

    uint8_t flag = pop_stack(stack).ui8;
    if(flag == 0)
        return stack->data[0].ptr + *(ip + 1);
    else
        return ip + 2;
}

uint8_t* opt_jump_less(uint8_t* ip, stack_t* stack) {
    if(peek_stack(stack).type != OBJECT_FLAG)
        exit(-10);

    uint8_t flag = pop_stack(stack).ui8;
    if(flag == 2)
        return stack->data[0].ptr + *(ip + 1);
    else
        return ip + 2;
}

uint8_t* opt_jump_grater(uint8_t* ip, stack_t* stack) {
    if(peek_stack(stack).type != OBJECT_FLAG)
        exit(-10);

    uint8_t flag = pop_stack(stack).ui8;
    if(flag == 1)
        return stack->data[0].ptr + *(ip + 1);
    else
        return ip + 2;
}

uint8_t* opt_argumentify(uint8_t* ip, stack_t* stack) {
    uint8_t amount = *(ip + 1);
    for(size_t i = 0; i < amount; i++){
        if(peek_stack(stack).type == OBJECT_UNSIGNED_8 || peek_stack(stack).type == OBJECT_SIGNED_8)
            *(ip + 3 + i) = pop_stack(stack).ui8;
        else
            exit(-10);
    }
    return ip + 2;
}

uint8_t* opt_sum(uint8_t* ip, stack_t* stack) {
    uint8_t first = *(ip + 1);
    uint8_t second = *(ip + 2);

    object_t o;
    o.type = OBJECT_SIGNED_8;
    o.i8 = first + second;
    push_stack(stack, o);

    return ip + 3;
}

uint8_t* opt_sub(uint8_t* ip, stack_t* stack) {
    uint8_t first = *(ip + 1);
    uint8_t second = *(ip + 2);

    object_t o;
    o.type = OBJECT_SIGNED_8;
    o.i8 = first - second;
    push_stack(stack, o);

    return ip + 3;
}

uint8_t* opt_multiply(uint8_t* ip, stack_t* stack) {
    uint8_t first = *(ip + 1);
    uint8_t second = *(ip + 2);

    object_t o;
    o.type = OBJECT_SIGNED_8;
    o.i8 = first * second;
    push_stack(stack, o);

    return ip + 3;
}

uint8_t* opt_divide (uint8_t* ip, stack_t* stack) {
    uint8_t first = *(ip + 1);
    uint8_t second = *(ip + 2);

    object_t o;
    o.type = OBJECT_FLOAT;
    o.i8 = first / second;
    push_stack(stack, o);

    return ip + 3;
}


void register_instructions(instruction* opt){
    for(size_t i = 0; i < 256; i++){
        opt[i] = opt_nop;
    }

    opt['.'] = opt_nop; // No operation. I move nothing and nothing moves me.
    opt['a'] = opt_argumentify; // Places the amount of elements specified by its argument from the stack in front of the next operation.
                                   // (Effectively making the data from the stack into that operations arguments)
                                   // NOTE: You must reserve space in the byte code for the data to be placed. Use the nop operation ('.') to reserve space.

    ////// Opt codes pushing raw data onto the stack or prints raw data//////
    opt['b'] = opt_push_byte; // Pushes its argument as unsigned byte to the stack.
    opt['s'] = opt_push_string; // Pushes the amount of bytes specified by its first arguments onto the stack (The bytes following the first argument are pushed).
    opt['e'] = opt_emit; // Pops one byte from the stack an writes it to standard out.
    opt['i'] = opt_emit_char; // Pops a element from the stack and prints es contents to standard as a ascii character.
    opt['p'] = opt_emit_string; // Pops the amount of bytes specified by its argument of the stack and writes them to standard out.

     ////// Opt codes which control the flow //////
    opt['j'] = opt_jump; // Jumps to the address specified in its argument.
    opt['<'] = opt_jump_less; // Executes a jump if the last compare determent that the first argument is smaller then the second argument of the compare.
    opt['>'] = opt_jump_grater; //  Executes a jump if the last compare determent that the first argument is grater then the second argument of the compare.
    opt['='] = opt_jump_equal; // Executes a jump if the last compare determent that the first and second argument of that compare are equal.
    opt['!'] = opt_jump_not_equal; // Executes a jump if the last compare determent that the first and second argument of that compare are not equal.

    ////// Logic controlling opt codes //////
    opt['c'] = opt_compare; // Pushes a Flag onto the stack which specifies if the two arguments are equal or if the first one is smaller/grater then the second.
    opt['+'] = opt_sum; // Sums the next two bytes and pushes the result onto the stack.
    opt['-'] = opt_sub; // Subtracts the next two bytes and pushes the result onto the stack.
    opt['*'] = opt_multiply; // Multiples the next two bytes and pushes the result onto the stack.
    opt['/'] = opt_divide; // Divides the next two bytes and pushes the result onto the stack.
    opt['+'] = opt_sum; // Sums the next two bytes and pushes the result onto the stack.
    opt['-'] = opt_sub; // Subtracts the next two bytes and pushes the result onto the stack.
    opt['*'] = opt_multiply; // Multiples the next two bytes and pushes the result onto the stack.
    opt['/'] = opt_divide; // Divides the next two bytes and pushes the result onto the stack.
}

#endif //VIRTUAL_MACHIEN_INSTRUCTIONS_H

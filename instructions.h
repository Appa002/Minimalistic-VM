//
// Created by a_mod on 10.05.2018.
//

#ifndef VIRTUAL_MACHIEN_INSTRUCTIONS_H
#define VIRTUAL_MACHIEN_INSTRUCTIONS_H

#include <memory.h>
#include "stack.h"

typedef uint8_t* (*instruction)(uint8_t * ip, stack_t* stack);

uint8_t* add_to_ip(uint8_t* ip, uint8_t amount){
    while (amount > 0){
        ip++;
        if(*ip == ';'){
            ip++;
            while (*ip != ';')
                ip++;
            ip++;
        }
        amount--;
    }
    return ip;
}

uint8_t* opt_nop(uint8_t* ip, stack_t* stack){
    return add_to_ip(ip, 1);
}

uint8_t* opt_push_8_bit(uint8_t *ip, stack_t *stack){
    object_t o;
    o.data = malloc(sizeof(uint8_t));
    *(uint8_t *) o.data = *add_to_ip(ip, 1);
    o.type = OBJECT_UNSIGNED;
    o.size = sizeof(uint8_t);

    push_stack(stack, o);
    return add_to_ip(ip, 2);
}

uint8_t* opt_push_32_bit(uint8_t* ip, stack_t* stack){
    object_t o;
    o.type = OBJECT_UNSIGNED;
    o.size = sizeof(uint32_t);
    o.data = malloc(sizeof(uint32_t));
    memcpy(o.data, add_to_ip(ip, 1), sizeof(uint32_t));
    push_stack(stack, o);
    return add_to_ip(ip, 2);
}

uint8_t* opt_emit(uint8_t* ip, stack_t* stack) {
    object_t o = peek_stack(stack);

    switch (o.type){
        case(OBJECT_FLOAT):
            printf("%.6f", pop_stack(float, stack));
            break;
        case(OBJECT_UNSIGNED):
            printf("%u", pop_stack(uint16_t, stack));
            break;
        case(OBJECT_SIGNED):
            printf("%d", pop_stack(int16_t, stack));
            break;
        default:
            break;
    }
    return add_to_ip(ip, 1);
}

uint8_t* opt_emit_char(uint8_t* ip, stack_t* stack){
    if(peek_stack(stack).type != OBJECT_UNSIGNED && peek_stack(stack).size != 1)
        exit(-10);

    printf("%c", pop_stack(uint8_t, stack));
    return add_to_ip(ip, 1);
}

uint8_t* opt_jump(uint8_t* ip, stack_t* stack){
    uint8_t arg = *(add_to_ip(ip, 1));
    return stack->data[0].data + arg;
}

uint8_t* opt_push_string(uint8_t* ip, stack_t* stack){
    uint8_t length_of_string = *add_to_ip(ip, 1);
    for(size_t i = 0; i < length_of_string; i++){
        object_t o;
        o.type = OBJECT_UNSIGNED;
        o.size = sizeof(uint8_t);
        o.data = malloc(sizeof(uint8_t));
        *(uint8_t*)o.data = *add_to_ip(ip, 2 + i);
        push_stack(stack, o);
    }

    return add_to_ip(ip, 2 + length_of_string);
}

uint8_t* opt_emit_string(uint8_t* ip, stack_t* stack) {
    uint8_t length_of_string = *(ip + 1);
    for (size_t i = 0; i < length_of_string; i++) {
        if (peek_stack(stack).type != OBJECT_UNSIGNED || peek_stack(stack).size != 1) {
            exit(-10);
        }

        char data[2];
        data[0] = pop_stack(uint8_t, stack);
        data[1] = 0;
        printf(data);
    }

    return add_to_ip(ip, 2);
}

uint8_t* opt_compare(uint8_t* ip, stack_t* stack) {
    uint8_t first = *(ip+1);
    uint8_t second = *(ip+2);

    object_t object;
    object.type = OBJECT_FLAG;
    object.size = sizeof(uint8_t);
    object.data = malloc(sizeof(uint8_t));

    if(first-second == 0){
        *(uint8_t*)object.data = 0;
    }else if(first-second > 0) {
        *(uint8_t*)object.data = 1;
    }else{
        *(uint8_t*)object.data = 2;
    }
    push_stack(stack, object);

    return add_to_ip(ip, 3);
}

uint8_t* opt_jump_not_equal(uint8_t* ip, stack_t* stack) {
    if(peek_stack(stack).type != OBJECT_FLAG)
        exit(-10);

    uint8_t flag = pop_stack(uint8_t, stack);
    uint8_t arg = *add_to_ip(ip, 1);

    if(flag != 0)
        return stack->data[0].data + arg;
    else
        return add_to_ip(ip, 2);
}

uint8_t* opt_jump_equal(uint8_t* ip, stack_t* stack) {
    if(peek_stack(stack).type != OBJECT_FLAG)
        exit(-10);

    uint8_t flag = pop_stack(uint8_t, stack);
    uint8_t arg = *add_to_ip(ip, 1);

    if(flag == 0)
        return stack->data[0].data + arg;
    else
        return add_to_ip(ip, 2);
}

uint8_t* opt_jump_less(uint8_t* ip, stack_t* stack) {
    if(peek_stack(stack).type != OBJECT_FLAG)
        exit(-10);

    uint8_t flag = pop_stack(uint8_t, stack);
    uint8_t arg = *add_to_ip(ip, 1);

    if(flag == 2)
        return stack->data[0].data + arg;
    else
        return add_to_ip(ip, 2);
}

uint8_t* opt_jump_grater(uint8_t* ip, stack_t* stack) {
    if(peek_stack(stack).type != OBJECT_FLAG)
        exit(-10);

    uint8_t flag = pop_stack(uint8_t, stack);
    uint8_t arg = *add_to_ip(ip, 1);

    if(flag == 1)
        return stack->data[0].data + arg;
    else
        return add_to_ip(ip, 2);
}

uint8_t* opt_argumentify(uint8_t* ip, stack_t* stack) {
    uint8_t amount = *add_to_ip(ip, 1);
    for(uint8_t i = 0; i < amount; i++){
        object_t o = peek_stack(stack);
        memcpy(add_to_ip(ip, 3 + i), o.data, o.size);
        remove_from_stack(stack);
    }
    return add_to_ip(ip, 1);
}

uint8_t* opt_sum(uint8_t* ip, stack_t* stack) {
    object_t o;
    o.size = sizeof(uint32_t); // sizeof(uint32_t) == sizeof(int32_t)
    o.data = malloc(sizeof(uint32_t));

    if(peek_stack(stack).type == OBJECT_UNSIGNED){
        uint32_t first = pop_stack(uint32_t, stack);

        if(peek_stack(stack).type == OBJECT_UNSIGNED){
            uint32_t second = pop_stack(uint32_t, stack);
            o.type = OBJECT_UNSIGNED;
            *(uint32_t*)o.data = first + second;
        }else{
            int32_t second = pop_stack(int32_t, stack);
            o.type = OBJECT_SIGNED;
            *(int32_t*)o.data = first + second;
        }

    }else{
        int32_t first = pop_stack(uint32_t, stack);

        if(peek_stack(stack).type == OBJECT_UNSIGNED){
            uint32_t second = pop_stack(uint32_t, stack);
            o.type = OBJECT_SIGNED;
            *(int32_t*)o.data = first + second;
        }else{
            int32_t second = pop_stack(int32_t, stack);
            o.type = OBJECT_SIGNED;
            *(int32_t*)o.data = first + second;
        }
    }

    push_stack(stack, o);
    return add_to_ip(ip, 1);
}

uint8_t* opt_sub(uint8_t* ip, stack_t* stack) {
    object_t o;
    o.size = sizeof(uint32_t); // sizeof(uint32_t) == sizeof(int32_t)
    o.data = malloc(sizeof(uint32_t));

    if(peek_stack(stack).type == OBJECT_UNSIGNED){
        uint32_t first = pop_stack(uint32_t, stack);

        if(peek_stack(stack).type == OBJECT_UNSIGNED){
            uint32_t second = pop_stack(uint32_t, stack);
            o.type = OBJECT_SIGNED;
            *(int32_t*)o.data = first - second;
        }else{
            int32_t second = pop_stack(int32_t, stack);
            o.type = OBJECT_SIGNED;
            *(int32_t*)o.data = first - second;
        }

    }else{
        int32_t first = pop_stack(uint32_t, stack);

        if(peek_stack(stack).type == OBJECT_UNSIGNED){
            uint32_t second = pop_stack(uint32_t, stack);
            o.type = OBJECT_SIGNED;
            *(int32_t*)o.data = first - second;
        }else{
            int32_t second = pop_stack(int32_t, stack);
            o.type = OBJECT_SIGNED;
            *(int32_t*)o.data = first - second;
        }
    }

    push_stack(stack, o);
    return add_to_ip(ip, 1);
}

uint8_t* opt_multiply(uint8_t* ip, stack_t* stack) {
    object_t o;
    o.size = sizeof(uint32_t); // sizeof(uint32_t) == sizeof(int32_t)
    o.data = malloc(sizeof(uint32_t));

    if(peek_stack(stack).type == OBJECT_UNSIGNED){
        uint32_t first = pop_stack(uint32_t, stack);

        if(peek_stack(stack).type == OBJECT_UNSIGNED){
            uint32_t second = pop_stack(uint32_t, stack);
            o.type = OBJECT_UNSIGNED;
            *(uint32_t*)o.data = first * second;
        }else{
            int32_t second = pop_stack(int32_t, stack);
            o.type = OBJECT_SIGNED;
            *(int32_t*)o.data = first * second;
        }

    }else{
        int32_t first = pop_stack(uint32_t, stack);

        if(peek_stack(stack).type == OBJECT_UNSIGNED){
            uint32_t second = pop_stack(uint32_t, stack);
            o.type = OBJECT_SIGNED;
            *(int32_t*)o.data = first * second;
        }else{
            int32_t second = pop_stack(int32_t, stack);
            o.type = OBJECT_SIGNED;
            *(int32_t*)o.data = first * second;
        }
    }

    push_stack(stack, o);
    return add_to_ip(ip, 1);
}

uint8_t* opt_divide (uint8_t* ip, stack_t* stack) {
    object_t o;
    o.size = sizeof(float); // sizeof(uint32_t) == sizeof(int32_t)
    o.data = malloc(sizeof(float));

    if (peek_stack(stack).type == OBJECT_UNSIGNED) {
        uint32_t first = pop_stack(uint32_t, stack);

        if (peek_stack(stack).type == OBJECT_UNSIGNED) {
            uint32_t second = pop_stack(uint32_t, stack);
            o.type = OBJECT_FLOAT;
            *(float *) o.data = first / second;
        } else if (peek_stack(stack).type == OBJECT_SIGNED) {
            int32_t second = pop_stack(int32_t, stack);
            o.type = OBJECT_FLOAT;
            *(float *) o.data = first / second;
        } else if (peek_stack(stack).type == OBJECT_FLOAT) {
            float second = pop_stack(float, stack);
            o.type = OBJECT_FLOAT;
            *(float *) o.data = first / second;
        }

    } else {
        int32_t first = pop_stack(uint32_t, stack);

        if (peek_stack(stack).type == OBJECT_UNSIGNED) {
            uint32_t second = pop_stack(uint32_t, stack);
            o.type = OBJECT_FLOAT;
            *(float *) o.data = first / second;
        } else if (peek_stack(stack).type == OBJECT_SIGNED) {
            int32_t second = pop_stack(int32_t, stack);
            o.type = OBJECT_FLOAT;
            *(float *) o.data = first / second;
        } else if (peek_stack(stack).type == OBJECT_FLOAT) {
            float second = pop_stack(float, stack);
            o.type = OBJECT_FLOAT;
            *(float *) o.data = first / second;
        }
    }

    push_stack(stack, o);
    return add_to_ip(ip, 1);
}

uint8_t* opt_call(uint8_t* ip, stack_t* stack){
    if(peek_stack(stack).type != OBJECT_UNSIGNED || peek_stack(stack).size != 4)
        exit(-10);
    uint32_t address = pop_stack(uint32_t, stack);
    return stack->data[0].data+ address;
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
    opt['b'] = opt_push_8_bit; // Pushes its argument as unsigned byte to the stack.
    opt['l'] = opt_push_32_bit; // Pushes its argument as 32 bit unsigned num on to the stack.
    opt['s'] = opt_push_string; // Pushes the amount of bytes specified by its first arguments onto the stack (The bytes following the first argument are pushed).
    opt['e'] = opt_emit; // Pops one byte from the stack an writes it to standard out.
    opt['i'] = opt_emit_char; // Pops a element from the stack and prints es contents to standard as a ascii character.
    opt['p'] = opt_emit_string; // Pops the amount of bytes specified by its argument of the stack and writes them to standard out.

     ////// Opt codes which control the flow //////
    opt['j'] = opt_jump; // Jumps to the address specified in its argument. Expects 32bit num
    opt['<'] = opt_jump_less; // Executes a jump if the last compare determent that the first argument is smaller then the second argument of the compare. Expects 32bit num
    opt['>'] = opt_jump_grater; //  Executes a jump if the last compare determent that the first argument is grater then the second argument of the compare. Expects 32bit num
    opt['='] = opt_jump_equal; // Executes a jump if the last compare determent that the first and second argument of that compare are equal. Expects 32bit num
    opt['!'] = opt_jump_not_equal; // Executes a jump if the last compare determent that the first and second argument of that compare are not equal. Expects 32bit num

    ////// Logic controlling opt codes //////
    opt['c'] = opt_compare; // Pushes a Flag onto the stack which specifies if the two arguments are equal or if the first one is smaller/grater then the second.
    opt['+'] = opt_sum; // Sums the next two bytes and pushes the result onto the stack.
    opt['-'] = opt_sub; // Subtracts the next two bytes and pushes the result onto the stack.
    opt['*'] = opt_multiply; // Multiples the next two bytes and pushes the result onto the stack.
    opt['/'] = opt_divide; // Divides the next two bytes and pushes the result onto the stack.
}

#endif //VIRTUAL_MACHIEN_INSTRUCTIONS_H

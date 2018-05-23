//
// Created by a_mod on 10.05.2018.
//

#ifndef VIRTUAL_MACHIEN_INSTRUCTIONS_H
#define VIRTUAL_MACHIEN_INSTRUCTIONS_H

#include <memory.h>
#include "stack.h"
#include "utils.h"
#include "object_maths.h"

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
    o.value = *add_to_ip(ip, 1);
    o.type = OBJECT_NUMBER;
    o.size = sizeof(uint8_t);
    o.signage = 0;

    push_stack(stack, o);
    return add_to_ip(ip, 2);
}

uint8_t* opt_push_16_bit(uint8_t* ip, stack_t* stack){
    object_t o;
    o.type = OBJECT_NUMBER;
    o.size = sizeof(uint16_t);
    o.value = 0x0;
    o.value = (ip[1] << 8) | (ip[2]);
    o.signage = 0;
    push_stack(stack, o);
    return add_to_ip(ip, 3);
}

uint8_t* opt_push_32_bit(uint8_t* ip, stack_t* stack){
    object_t o;
    o.type = OBJECT_NUMBER;
    o.size = sizeof(uint32_t);
    o.value = 0x0;
    o.value = (ip[1] << 31) | (ip[2] << 16) | (ip[3] << 8) | (ip[4]);
    o.signage = 0;
    push_stack(stack, o);
    return add_to_ip(ip, 5);
}

uint8_t* opt_emit(uint8_t* ip, stack_t* stack) {
    object_t o = peek_stack(stack);

    switch (o.type){
        case(OBJECT_FLOAT):
            printf("%.6f", (float)pop_stack(stack).value);
            break;
        case(OBJECT_NUMBER):
            (peek_stack(stack).signage == 0) ? (printf("%u", (unsigned)pop_stack(stack).value)) : ((printf("-%u", (unsigned)pop_stack(stack).value)));
            break;
        default:
            break;
    }
    return add_to_ip(ip, 1);
}

uint8_t* opt_emit_char(uint8_t* ip, stack_t* stack){
    if(peek_stack(stack).type != OBJECT_NUMBER && peek_stack(stack).size != sizeof(uint8_t))
        exit(-10);

    printf("%c", (uint8_t)pop_stack(stack).value);
    return add_to_ip(ip, 1);
}

uint8_t* opt_jump(uint8_t* ip, stack_t* stack){
    uint8_t arg = *(add_to_ip(ip, 1));
    return add_to_ip(stack->data[0].ptr, arg);
}

uint8_t* opt_push_string(uint8_t* ip, stack_t* stack){
    uint8_t length_of_string = *add_to_ip(ip, 1);
    for(size_t i = 0; i < length_of_string; i++){
        object_t o;
        o.type = OBJECT_NUMBER;
        o.size = sizeof(uint8_t);
        o.value = *add_to_ip(ip, 2 + i);
        push_stack(stack, o);
    }

    return add_to_ip(ip, 2 + length_of_string);
}

uint8_t* opt_emit_string(uint8_t* ip, stack_t* stack) {
    uint8_t length_of_string = *(ip + 1);
    for (size_t i = 0; i < length_of_string; i++) {
        if (peek_stack(stack).type != OBJECT_NUMBER || peek_stack(stack).size != 1) {
            exit(-10);
        }

        char data[2];
        data[0] = (uint8_t)pop_stack(stack).value;
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

    if(first-second == 0){
        object.value = 0;
    }else if(first-second > 0) {
        object.value = 1;
    }else{
        object.value = 2;
    }
    push_stack(stack, object);

    return add_to_ip(ip, 3);
}

uint8_t* opt_jump_not_equal(uint8_t* ip, stack_t* stack) {
    if(peek_stack(stack).type != OBJECT_FLAG)
        exit(-10);

    uint8_t flag = (uint8_t)pop_stack(stack).value;
    uint8_t arg = *add_to_ip(ip, 1);

    if(flag != 0)
        return add_to_ip(stack->data[0].ptr, arg);
    else
        return add_to_ip(ip, 2);
}

uint8_t* opt_jump_equal(uint8_t* ip, stack_t* stack) {
    if(peek_stack(stack).type != OBJECT_FLAG)
        exit(-10);

    uint8_t flag = (uint8_t)pop_stack(stack).value;
    uint8_t arg = *add_to_ip(ip, 1);

    if(flag == 0)
        return add_to_ip(stack->data[0].ptr, arg);
    else
        return add_to_ip(ip, 2);
}

uint8_t* opt_jump_less(uint8_t* ip, stack_t* stack) {
    if(peek_stack(stack).type != OBJECT_FLAG)
        exit(-10);

    uint8_t flag = (uint8_t)pop_stack(stack).value;
    uint8_t arg = *add_to_ip(ip, 1);

    if(flag == 2)
        return add_to_ip(stack->data[0].ptr, arg);
    else
        return add_to_ip(ip, 2);
}

uint8_t* opt_jump_grater(uint8_t* ip, stack_t* stack) {
    if(peek_stack(stack).type != OBJECT_FLAG)
        exit(-10);

    uint8_t flag = (uint8_t)pop_stack(stack).value;
    uint8_t arg = *add_to_ip(ip, 1);

    if(flag == 1)
        return add_to_ip(stack->data[0].ptr, arg);
    else
        return add_to_ip(ip, 2);
}

uint8_t* opt_argumentify(uint8_t* ip, stack_t* stack) {
    uint8_t amount = *add_to_ip(ip, 1);
    for(uint8_t i = 0; i < amount; i++){
        object_t o = peek_stack(stack);
        memcpy(add_to_ip(ip, 3 + i), &o.value, o.size);
        remove_from_stack(stack);
    }
    return add_to_ip(ip, 2);
}

uint8_t* opt_sum(uint8_t* ip, stack_t* stack) {
    object_t o = add(pop_stack(stack), pop_stack(stack));
    push_stack(stack, o);
    return add_to_ip(ip, 1);
}

uint8_t* opt_sub(uint8_t* ip, stack_t* stack) {
    object_t o;
    o = subtract(pop_stack(stack), pop_stack(stack));
    push_stack(stack, o);
    return add_to_ip(ip, 1);
}

uint8_t* opt_multiply(uint8_t* ip, stack_t* stack) {
    object_t o;
    o = multiply(pop_stack(stack), pop_stack(stack));
    push_stack(stack, o);
    return add_to_ip(ip, 1);
}

uint8_t* opt_divide (uint8_t* ip, stack_t* stack) {
    object_t a = pop_stack(stack);
    object_t b = pop_stack(stack);
    object_t o = divide(a, b);
    push_stack(stack, o);
    return add_to_ip(ip, 1);
}

uint8_t* opt_call(uint8_t* ip, stack_t* stack){
    if(peek_stack(stack).type != OBJECT_NUMBER || peek_stack(stack).size != sizeof(uint32_t))
        exit(-10);
    uint32_t address = (uint32_t)pop_stack(stack).value;

    object_t o;
    o.type = OBJECT_POINTER;
    o.size = sizeof(intptr_t);
    o.value = (uint32_t)((intptr_t)(ip + 1) - (intptr_t)(stack->data[0].ptr));
    push_stack(stack, o);

    return stack->data[0].ptr + address;
}

uint8_t* opt_return(uint8_t* ip, stack_t* stack){
    if(peek_stack(stack).type != OBJECT_POINTER || peek_stack(stack).size != sizeof(intptr_t))
        exit(-10);
    uint32_t address = (uint32_t)pop_stack(stack).value;
    return stack->data[0].ptr + address;
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
    opt['q'] = opt_push_16_bit; // Pushes its argument as 16 bit unsigned num on to the stack.
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
    opt['g'] = opt_call;
    opt['r'] = opt_return;

    ////// Logic controlling opt codes //////
    opt['c'] = opt_compare; // Pushes a Flag onto the stack which specifies if the two arguments are equal or if the first one is smaller/grater then the second.
    opt['+'] = opt_sum; // Sums the next two bytes and pushes the result onto the stack.
    opt['-'] = opt_sub; // Subtracts the next two bytes and pushes the result onto the stack.
    opt['*'] = opt_multiply; // Multiples the next two bytes and pushes the result onto the stack.
    opt['/'] = opt_divide; // Divides the next two bytes and pushes the result onto the stack.
}

#endif //VIRTUAL_MACHIEN_INSTRUCTIONS_H

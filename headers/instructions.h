//
// Created by a_mod on 10.05.2018.
//

#ifndef VIRTUAL_MACHIEN_INSTRUCTIONS_H
#define VIRTUAL_MACHIEN_INSTRUCTIONS_H

#include "stack.h"

typedef uint8_t* (*instruction)(uint8_t * ip, stack_t* stack);

uint8_t* add_to_ip(uint8_t* ip, uint32_t amount);

uint8_t* opt_nop(uint8_t* ip, stack_t* stack);

uint8_t* opt_push_8_bit(uint8_t *ip, stack_t *stack);

uint8_t* opt_push_16_bit(uint8_t* ip, stack_t* stack);

uint8_t* opt_push_32_bit(uint8_t* ip, stack_t* stack);

uint8_t* opt_emit(uint8_t* ip, stack_t* stack);

uint8_t* opt_emit_char(uint8_t* ip, stack_t* stack);

uint8_t* opt_jump(uint8_t* ip, stack_t* stack);

uint8_t* opt_push_string(uint8_t* ip, stack_t* stack);

uint8_t* opt_emit_string(uint8_t* ip, stack_t* stack);

uint8_t* opt_compare(uint8_t* ip, stack_t* stack);

uint8_t* opt_jump_not_equal(uint8_t* ip, stack_t* stack);

uint8_t* opt_jump_equal(uint8_t* ip, stack_t* stack);

uint8_t* opt_jump_less(uint8_t* ip, stack_t* stack);

uint8_t* opt_jump_grater(uint8_t* ip, stack_t* stack);

uint8_t* opt_sum(uint8_t* ip, stack_t* stack);

uint8_t* opt_sub(uint8_t* ip, stack_t* stack);

uint8_t* opt_multiply(uint8_t* ip, stack_t* stack);

uint8_t* opt_divide (uint8_t* ip, stack_t* stack);

uint8_t* opt_call(uint8_t* ip, stack_t* stack);

uint8_t* opt_return(uint8_t* ip, stack_t* stack);

uint8_t* opt_read_from_stack(uint8_t* ip, stack_t* stack);

uint8_t* opt_write_to_stack(uint8_t* ip, stack_t* stack);

uint8_t* opt_pop_from_stack(uint8_t *ip, stack_t *stack);

void register_instructions(instruction* opt);

#endif //VIRTUAL_MACHIEN_INSTRUCTIONS_H

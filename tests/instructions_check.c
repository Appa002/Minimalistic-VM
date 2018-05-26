#include "unit_framework.h"
#include "../headers/instructions.h"

uint8_t* execute(uint8_t* ip, stack_t* stack){
    instruction* opt = malloc(sizeof(instruction*) * 256);
    register_instructions(opt);
    unsigned i = 0;

    while(*ip != 'h' && i < 25){
        ip = opt[*ip](ip, stack);
        i++;
    }
    free(opt);
    return ip;
}

int unit_test_push_8_bit() {
    uint8_t code[3] = {'b', 0x3, 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);

    ip = opt_push_8_bit(ip, &stack);
    ASSERT_EQUAL(*ip, 'h');
    ASSERT_EQUAL((uint8_t)peek_stack(&stack).value, 0x3);
    ASSERT_EQUAL(peek_stack(&stack).signage, 0);
    del_stack(&stack);
    return 0;
}

int unit_test_push_16_bit() {
    uint8_t code[] = {'q', 0x0, 0x3, 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);

    ip = opt_push_16_bit(ip, &stack);
    ASSERT_EQUAL(*ip, 'h');
    ASSERT_EQUAL((uint16_t)peek_stack(&stack).value, 0x3);
    ASSERT_EQUAL(peek_stack(&stack).signage, 0);
    del_stack(&stack);
    return 0;
}

int unit_test_push_32_bit() {
    uint8_t code[] = {'q', 0x0, 0x0, 0x0, 0x3, 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);

    ip = opt_push_32_bit(ip, &stack);
    ASSERT_EQUAL(*ip, 'h');
    ASSERT_EQUAL((uint16_t)peek_stack(&stack).value, 0x3);
    ASSERT_EQUAL(peek_stack(&stack).signage, 0);
    del_stack(&stack);
    return 0;
}

int unit_test_emit() {
    /*No clue how to unit test printf!*/
}

int unit_test_emit_char() {
    /*No clue how to unit test printf!*/
}

int unit_test_jump() {
    uint8_t code[] = {'l', 0x0, 0x0, 0x0, 8, 'j', '.', '.', 'k', 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);
    stack.data[0].ptr = ip;

    ip = opt_push_32_bit(ip, &stack);
    ip = opt_jump(ip, &stack);
    ASSERT_EQUAL(*ip, 'k');
    del_stack(&stack);
    return 0;
}

int unit_test_push_string() {
    uint8_t code[] = {'s', 0x3, 'h', 'i', '!', 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);

    ip = opt_push_string(ip, &stack);
    ASSERT_EQUAL(*ip, 'h');
    ASSERT_EQUAL((uint8_t)pop_stack(&stack).value, '!');
    ASSERT_EQUAL((uint8_t)pop_stack(&stack).value, 'i');
    ASSERT_EQUAL((uint8_t)pop_stack(&stack).value, 'h');
    del_stack(&stack);
    return 0;
}

int unit_test_emit_string() {
    /*No clue how to unit test printf!*/
}

int unit_test_compare() {
    uint8_t code[] = {'b', 0x3, 'b', 0x3,'c', 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);

    ip = execute(ip, &stack);

    ASSERT_EQUAL(*ip, 'h');
    ASSERT_EQUAL((uint8_t)pop_stack(&stack).value, 0);

    code[1] = 0x2;
    ip = code;

    ip = execute(ip, &stack);

    ASSERT_EQUAL(*ip, 'h');
    ASSERT_EQUAL((uint8_t)pop_stack(&stack).value, 1);

    code[1] = 0x4;
    ip = code;

    ip = execute(ip, &stack);

    ASSERT_EQUAL(*ip, 'h');
    ASSERT_EQUAL((uint8_t)pop_stack(&stack).value, 2);

    del_stack(&stack);

    return 0;
}

int unit_test_jump_not_equal() {
    uint8_t code[] = {'b', 0, 'b', 1, 'c', 'l', 0, 0, 0, 12,'!', '.', 'k', 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);
    stack.data[0].ptr = ip;

    ip = opt_push_8_bit(ip, &stack);
    ip = opt_push_8_bit(ip, &stack);
    ip = opt_compare(ip, &stack);
    ip = opt_push_32_bit(ip, &stack);
    ip = opt_jump_not_equal(ip, &stack);
    ASSERT_EQUAL(*ip, 'k');

    code[1] = 1;
    ip = code;

    ip = opt_push_8_bit(ip, &stack);
    ip = opt_push_8_bit(ip, &stack);
    ip = opt_compare(ip, &stack);
    ip = opt_push_32_bit(ip, &stack);
    ip = opt_jump_not_equal(ip, &stack);
    ASSERT_EQUAL(*ip, '.');

    del_stack(&stack);
    return 0;
}

int unit_test_jump_equal() {
    uint8_t code[] = {'b', 0, 'b', 0, 'c', 'l', 0, 0, 0, 12,'!', '.', 'k', 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);
    stack.data[0].ptr = ip;

    ip = opt_push_8_bit(ip, &stack);
    ip = opt_push_8_bit(ip, &stack);
    ip = opt_compare(ip, &stack);
    ip = opt_push_32_bit(ip, &stack);
    ip = opt_jump_equal(ip, &stack);
    ASSERT_EQUAL(*ip, 'k');

    code[1] = 1;
    ip = code;

    ip = opt_push_8_bit(ip, &stack);
    ip = opt_push_8_bit(ip, &stack);
    ip = opt_compare(ip, &stack);
    ip = opt_push_32_bit(ip, &stack);
    ip = opt_jump_equal(ip, &stack);
    ASSERT_EQUAL(*ip, '.');

    return 0;
}

int unit_test_jump_less() {
    uint8_t code[] = {'b', 4, 'b', 3, 'c', 'l', 0, 0, 0, 12,'!', '.', 'k', 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);
    stack.data[0].ptr = ip;

    ip = opt_push_8_bit(ip, &stack);
    ip = opt_push_8_bit(ip, &stack);
    ip = opt_compare(ip, &stack);
    ip = opt_push_32_bit(ip, &stack);
    ip = opt_jump_less(ip, &stack);
    ASSERT_EQUAL(*ip, 'k');

    code[1] = 1;
    ip = code;

    ip = opt_push_8_bit(ip, &stack);
    ip = opt_push_8_bit(ip, &stack);
    ip = opt_compare(ip, &stack);
    ip = opt_push_32_bit(ip, &stack);
    ip = opt_jump_less(ip, &stack);
    ASSERT_EQUAL(*ip, '.');

    del_stack(&stack);
    return 0;
}

int unit_test_jump_grater() {
    uint8_t code[] = {'b', 0, 'b', 1, 'c', 'l', 0, 0, 0, 12,'!', '.', 'k', 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);
    stack.data[0].ptr = ip;

    ip = opt_push_8_bit(ip, &stack);
    ip = opt_push_8_bit(ip, &stack);
    ip = opt_compare(ip, &stack);
    ip = opt_push_32_bit(ip, &stack);
    ip = opt_jump_grater(ip, &stack);
    ASSERT_EQUAL(*ip, 'k');

    code[1] = 1;
    ip = code;

    ip = opt_push_8_bit(ip, &stack);
    ip = opt_push_8_bit(ip, &stack);
    ip = opt_compare(ip, &stack);
    ip = opt_push_32_bit(ip, &stack);
    ip = opt_jump_grater(ip, &stack);
    ASSERT_EQUAL(*ip, '.');


    del_stack(&stack);
    return 0;
}

int unit_test_sum() {
    uint8_t code[] = {'b', 0x1,'b', 1, '+', 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);

    object_t o;
    o.type = OBJECT_NUMBER;
    o.size = sizeof(uint16_t);
    o.value = 13;
    o.signage = 1;
    push_stack(&stack, o);
    ip = ip + 2;

    ip = opt_push_8_bit(ip, &stack);
    ip = opt_sum(ip, &stack);
    ASSERT_EQUAL(*ip, 'h');
    ASSERT_EQUAL((uint8_t)peek_stack(&stack).value, 12);
    return 0;
}

int unit_test_sub() {
    uint8_t code[] = {'b', 0x3, 'b', 0x1, '-', 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);

    ip = opt_push_8_bit(ip, &stack);
    ip = opt_push_8_bit(ip, &stack);
    ip = opt_sub(ip, &stack);
    ASSERT_EQUAL(*ip, 'h');
    ASSERT_EQUAL((uint8_t)peek_stack(&stack).value, 2);
    ASSERT_EQUAL(peek_stack(&stack).signage, 0);
    return 0;
}

int unit_test_multiply() {
    uint8_t code[] = {'b', 0x3, 'b', 0x1, '*', 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);

    ip = opt_push_8_bit(ip, &stack);
    ip = opt_push_8_bit(ip, &stack);
    ip = opt_multiply(ip, &stack);
    ASSERT_EQUAL(*ip, 'h');
    ASSERT_EQUAL((uint8_t)peek_stack(&stack).value, 0x3);
    return 0;
}

int unit_test_divide() {
    uint8_t code[] = {'b', 2, 'b', 3, '/', 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);

    ip = opt_push_8_bit(ip, &stack);
    ip = opt_push_8_bit(ip, &stack);
    ip = opt_divide(ip, &stack);
    ASSERT_EQUAL(*ip, 'h');
    ASSERT_EQUAL(peek_stack(&stack).value, 1.5f);
    ASSERT_EQUAL(peek_stack(&stack).signage, 0);
    return 0;
}

int unit_test_call() {
    uint8_t code[] = {'l', 0, 0, 0, 0, 'c', 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);
    stack.data[0].ptr = ip;

    ip = opt_push_32_bit(ip, &stack);
    ASSERT_EQUAL(*ip, 'c');
    ip = opt_call(ip, &stack);

    ASSERT_EQUAL(*ip, 'l');
    return 0;
}

int unit_test_return() {
    uint8_t code[] = {'l', 0, 0, 0, 0, 'r', 'c', 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);
    stack.data[0].ptr = ip;

    ip = opt_push_32_bit(ip, &stack);
    ASSERT_EQUAL(*ip, 'r');
    ip++;
    ip = opt_call(ip, &stack);
    ASSERT_EQUAL(*ip, 'l');
    ip = opt_push_32_bit(ip, &stack);
    pop_stack(&stack);
    ASSERT_EQUAL(*ip, 'r');
    ip = opt_return(ip, &stack);
    ASSERT_EQUAL(*ip, 'h');

    return 0;
}

int main() {
    register_test(unit_test_push_8_bit);
    register_test(unit_test_push_16_bit);
    register_test(unit_test_push_32_bit);
    register_test(unit_test_emit);
    register_test(unit_test_emit_char);
    register_test(unit_test_jump);
    register_test(unit_test_push_string);
    register_test(unit_test_emit_string);
    register_test(unit_test_compare);
    register_test(unit_test_jump_not_equal);
    register_test(unit_test_jump_equal);
    register_test(unit_test_jump_less);
    register_test(unit_test_jump_grater);
    register_test(unit_test_sub);
    register_test(unit_test_sum);
    register_test(unit_test_multiply);
    register_test(unit_test_divide);
    register_test(unit_test_call);
    register_test(unit_test_return);

    start_unit_test();
    end_unit_test();

    return 0;
}
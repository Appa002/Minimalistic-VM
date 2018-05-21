#include "unit_framework.h"
#include "../instructions.h"

int unit_test_push_8_bit() {
    uint8_t code[3] = {'b', 0x3, 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);

    ip = opt_push_8_bit(ip, &stack);
    ASSERT_EQUAL(*ip, 'h');
    ASSERT_EQUAL(*(uint8_t*)peek_stack(&stack).data, 0x3);
    del_stack(&stack);
    return 0;
}

int unit_test_push_16_bit() {
    uint8_t code[] = {'q', 0x0, 0x3, 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);

    ip = opt_push_16_bit(ip, &stack);
    ASSERT_EQUAL(*ip, 'h');
    ASSERT_EQUAL(*(uint16_t*)peek_stack(&stack).data, 0x3);
    del_stack(&stack);
    return 0;
}

int unit_test_push_32_bit() {
    uint8_t code[] = {'q', 0x0, 0x0, 0x0, 0x3, 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);

    ip = opt_push_32_bit(ip, &stack);
    ASSERT_EQUAL(*ip, 'h');
    ASSERT_EQUAL(*(uint16_t*)peek_stack(&stack).data, 0x3);
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
    uint8_t code[] = {'j', 0x4, '.', '.', 'k', 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);
    stack.data[0].data = ip;

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
    ASSERT_EQUAL(*(uint8_t*)peek_stack(&stack).data, '!');
    remove_from_stack(&stack);
    ASSERT_EQUAL(*(uint8_t*)peek_stack(&stack).data, 'i');
    remove_from_stack(&stack);
    ASSERT_EQUAL(*(uint8_t*)peek_stack(&stack).data, 'h');
    del_stack(&stack);
    return 0;
}

int unit_test_emit_string() {
    /*No clue how to unit test printf!*/
}

int unit_test_compare() {
    uint8_t code[] = {'c', 0x3, 0x3, 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);

    ip = opt_compare(ip, &stack);
    ASSERT_EQUAL(*ip, 'h');
    ASSERT_EQUAL(*(uint8_t*)peek_stack(&stack).data, 0);

    code[1] = 0x4;
    ip = code;
    ip = opt_compare(ip, &stack);
    ASSERT_EQUAL(*ip, 'h');
    ASSERT_EQUAL(*(uint8_t*)peek_stack(&stack).data, 1);

    code[1] = 0x2;
    ip = code;
    ip = opt_compare(ip, &stack);
    ASSERT_EQUAL(*ip, 'h');
    ASSERT_EQUAL(*(uint8_t*)peek_stack(&stack).data, 2);

    del_stack(&stack);

    return 0;
}

int unit_test_jump_not_equal() {
    uint8_t code[] = {'c', 0x4, 0x3, '!', 6, '.', 'k', 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);
    stack.data[0].data = ip;

    ip = opt_compare(ip, &stack);
    ip = opt_jump_not_equal(ip, &stack);
    ASSERT_EQUAL(*ip, 'k');

    code[1] = 0x3;
    ip = code;
    ip = opt_compare(ip, &stack);
    ip = opt_jump_not_equal(ip, &stack);
    ASSERT_EQUAL(*ip, '.');

    del_stack(&stack);
    return 0;
}

int unit_test_jump_equal() {
    uint8_t code[] = {'c', 0x4, 0x3, '=', 6, '.', 'k', 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);
    stack.data[0].data = ip;

    ip = opt_compare(ip, &stack);
    ip = opt_jump_equal(ip, &stack);
    ASSERT_EQUAL(*ip, '.');

    code[1] = 0x3;
    ip = code;
    ip = opt_compare(ip, &stack);
    ip = opt_jump_equal(ip, &stack);
    ASSERT_EQUAL(*ip, 'k');

    del_stack(&stack);
    return 0;
}

int unit_test_jump_less() {
    uint8_t code[] = {'c', 0x3, 0x4, '<', 6, '.', 'k', 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);
    stack.data[0].data = ip;

    ip = opt_compare(ip, &stack);
    ip = opt_jump_less(ip, &stack);
    ASSERT_EQUAL(*ip, 'k');

    code[2] = 0x3;
    ip = code;
    ip = opt_compare(ip, &stack);
    ip = opt_jump_less(ip, &stack);
    ASSERT_EQUAL(*ip, '.');

    del_stack(&stack);
    return 0;
}

int unit_test_jump_grater() {
    uint8_t code[] = {'c', 0x4, 0x3, '>', 6, '.', 'k', 'h'};
    uint8_t* ip = code;
    stack_t stack = new_stack(1024);
    stack.data[0].data = ip;

    ip = opt_compare(ip, &stack);
    ip = opt_jump_grater(ip, &stack);
    ASSERT_EQUAL(*ip, 'k');

    code[1] = 0x3;
    ip = code;
    ip = opt_compare(ip, &stack);
    ip = opt_jump_grater(ip, &stack);
    ASSERT_EQUAL(*ip, '.');

    del_stack(&stack);
    return 0;
}

int unit_test_argumentify() {

}

int unit_test_sum() {

}

int unit_test_sub() {

}

int unit_test_multiply() {

}

int unit_test_divide() {

}

int unit_test_call() {

}

int unit_test_return() {

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
    register_test(unit_test_argumentify);
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
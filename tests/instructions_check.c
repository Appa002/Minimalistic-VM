#include "unit_framework.h"
#include "../instructions.h"

int unit_test_push_8_bit() {

}

int unit_test_push_16_bit() {
}

int unit_test_push_32_bit() {

}

int unit_test_emit() {

}

int unit_test_emit_char() {

}

int unit_test_jump() {

}

int unit_test_push_string() {

}

int unit_test_emit_string() {

}

int unit_test_compare() {

}

int unit_test_jump_not_equal() {

}

int unit_test_jump_equal() {

}

int unit_test_jump_less() {

}

int unit_test_jump_grater() {

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
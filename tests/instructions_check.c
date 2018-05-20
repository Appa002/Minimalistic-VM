#include "unit_framework.h"

int unit_0(){
}

int main(){
    register_test(unit_0);
    start_unit_test();
    end_unit_test();

    return 0;
}
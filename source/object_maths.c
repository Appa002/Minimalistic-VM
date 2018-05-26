//
// Created by a_mod on 26.05.2018.
//

#include "../headers/object_maths.h"

double underflow_save_subtract(double a, double b, unsigned* signage){
    *signage = (a > b) ? (0) : (1);
    if(a > b)
        return a - b;
    else
        return b - a;
}

bool larger_then(object_t a, object_t b) {
    if (a.signage == b.signage)
        return ((int32_t) (a.value - b.value) > 0) ? (true) : (false);
    else if (a.signage == 1)
        return false;
    return true;
}

object_t add(object_t a, object_t b){
    object_t out;
    double result;
    double a_value = a.value;
    double b_value = b.value;

    if(a.type == OBJECT_FLOAT)
        a_value = a.double_value;

    if(b.type == OBJECT_FLOAT)
        b_value = b.double_value;

    if(a.signage == b.signage){
        result = a_value + b_value;
        out.signage = a.signage;
    }else{
        result = underflow_save_subtract(a_value, b_value, &out.signage);
    }

    out.type = OBJECT_NUMBER;
    out.value = (bitarr_t)result;
    if(a.type == OBJECT_FLOAT || b.type == OBJECT_FLOAT){
        out.type = OBJECT_FLOAT;
        out.double_value = result;
    }

    out.size = (a.size > b.size) ? (a.size) : (b.size);

    return out;
}

object_t subtract(object_t a, object_t b){
    object_t out;
    double result;
    double a_value = a.value;
    double b_value = b.value;

    out.type = OBJECT_NUMBER;
    out.size = (a.size > b.size) ? (a.size) : (b.size);

    if (a.type == OBJECT_FLOAT){
        a_value = a.double_value;
        out.type = OBJECT_FLOAT;
    }
    if (b.type == OBJECT_FLOAT){
        b_value = b.double_value;
        out.type = OBJECT_FLOAT;
    }

    if(a.signage == b.signage){
        if(a.signage == 1){
            b.signage = 0;
            return add(a, b);
        }else{
            result = underflow_save_subtract(a_value, b_value, &out.signage);
        }
    }else{
        result = a_value + b_value;
        out.signage = (a.signage == 1) ? (1) : (0);
    }

    out.value = (bitarr_t)result;
    if(a.type == OBJECT_FLOAT || b.type == OBJECT_FLOAT)
        out.double_value = result;

    return out;
}

object_t multiply(object_t a, object_t b){
    object_t out;
    double a_value = a.value;
    double b_value = b.value;
    double result;

    if(a.type == OBJECT_FLOAT)
        a_value = a.double_value;
    if(a.type == OBJECT_FLOAT)
        b_value = b.double_value;

    result = a_value * b_value;
    out.size = (a.size > b.size) ? (a.size) : (b.size);
    out.signage = (a.signage == b.signage) ? (0) : (1);

    out.type = OBJECT_NUMBER;
    out.value = (bitarr_t)result;
    if(a.type == OBJECT_FLOAT || b.type == OBJECT_FLOAT){
        out.double_value = result;
        out.type = OBJECT_FLOAT;
    }

    return out;
}

object_t divide(object_t a, object_t b){
    object_t out;

    double a_value = a.value;
    double b_value = b.value;

    if(a.type == OBJECT_FLOAT)
        a_value = a.double_value;
    if(a.type == OBJECT_FLOAT)
        b_value = b.double_value;

    out.double_value = a_value / b_value;
    out.type = OBJECT_FLOAT;

    if((bitarr_t)out.double_value == out.double_value)
        out.type = OBJECT_NUMBER;

    out.size = sizeof(uint32_t);
    out.signage = (a.signage != b.signage) ? (1) : (0);
    return out;
}


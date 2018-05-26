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

    if(a.signage == b.signage){
        out.value = a.value + a.value;
        out.signage = a.signage;
    }else{
        out.value = underflow_save_subtract(a.value, b.value, &out.signage);
    }

    out.type = (a.type == OBJECT_FLOAT || b.type == OBJECT_FLOAT) ? (OBJECT_FLOAT) : (OBJECT_NUMBER);
    out.size = (a.size > b.size) ? (a.size) : (b.size);

    return out;
}

object_t subtract(object_t a, object_t b){
    object_t out;

    if(a.signage == b.signage){
        if(a.signage == 1){
            b.signage = 0;
            out = add(a, b);
        }else{
            out.value = underflow_save_subtract(a.value, b.value, &out.signage);
        }
    }else{
        out.value = a.value + b.value;
        out.signage = (a.signage == 1) ? (1) : (0);
    }

    out.type = (a.type == OBJECT_FLOAT || b.type == OBJECT_FLOAT) ? (OBJECT_FLOAT) : (OBJECT_NUMBER);
    out.size = (a.size > b.size) ? (a.size) : (b.size);

    return out;
}

object_t multiply(object_t a, object_t b){
    object_t out;

    out.value = a.value * b.value;
    out.size = (a.size > b.size) ? (a.size) : (b.size);
    out.signage = (a.signage == b.signage) ? (0) : (1);
    out.type = (a.type == OBJECT_FLOAT || b.type == OBJECT_FLOAT) ? (OBJECT_FLOAT) : (OBJECT_NUMBER);

    return out;
}

object_t divide(object_t a, object_t b){
    object_t out;

    out.value = a.value / b.value;
    out.type = (a.type == OBJECT_FLOAT || b.type == OBJECT_FLOAT) ? (OBJECT_FLOAT) : (OBJECT_NUMBER);
    out.size = sizeof(double);
    out.signage = (a.signage != b.signage) ? (1) : (0);
    return out;
}


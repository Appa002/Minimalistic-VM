//
// Created by a_mod on 22.05.2018.
//

#ifndef VIRTUAL_MACHIEN_OBJECT_MATHS_H
#define VIRTUAL_MACHIEN_OBJECT_MATHS_H

#include "object.h"

object_t add(object_t a, object_t b);

bitarr_t underflow_save_subtract(bitarr_t a, bitarr_t b, unsigned* signage){
    *signage = (a > b) ? (0) : (1);
    if(a > b)
        return a - b;
    else
        return b - a;
}

object_t _add_number(object_t a, object_t b){
    object_t out;
    if(a.signage == b.signage){
        out.value = a.value + b.value;
        out.signage = a.signage;
    }else{
        out.value = underflow_save_subtract(a.value, b.value, &out.signage);
    }

    out.type = OBJECT_NUMBER;
    out.size = (a.size > b.size) ? (a.size) : (b.size);
    return out;
}

object_t _add_float(object_t a, object_t b){

}

object_t _subtract_number(object_t a, object_t b){
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

    out.size = (a.size > b.size) ? (a.size) : (b.size);
    out.type = OBJECT_NUMBER;
    return out;
}

object_t _subtract_float(object_t a, object_t b){

}

object_t _multiply_number(object_t a, object_t b){
    object_t out;
    out.value = a.value * b.value;
    out.size = (a.size > b.size) ? (a.size) : (b.size);
    out.signage = (a.signage == b.signage) ? (0) : (1);
    out.type = OBJECT_NUMBER;
    return out;
}

object_t _multiply_float(object_t a, object_t b){

}

object_t _divide_number(object_t a, object_t b){
    object_t out;
    out.type = OBJECT_FLOAT;
    out.size = sizeof(uint32_t);
    out.signage = (a.signage != b.signage) ? (1) : (0);
    uint32_t value = (uint32_t)((float)(a.value / b.value) * pow(10, 8));
    uint32_t comma_pos = get_last_significant_bit_index((bitarr_t) (a.value / b.value));

    out.value = write_at(out.value, value, 0, CHAR_BIT * 8);
    out.value = write_at(out.value, comma_pos, (sizeof(uint32_t) * CHAR_BIT / 2), sizeof(uint32_t) * CHAR_BIT);
    return out;
}

object_t _divide_float(object_t a, object_t b){

}
object_t add(object_t a, object_t b) {
    return _add_number(a, b);
}
object_t subtract (object_t a, object_t b){
    return _subtract_number(a, b);
}

object_t multiply (object_t a, object_t b){
    return _multiply_number(a, b);
}

object_t divide (object_t a, object_t b){
    return _divide_number(a, b);
}

#endif //VIRTUAL_MACHIEN_OBJECT_MATHS_H

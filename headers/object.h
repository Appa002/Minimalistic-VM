//
// Created by a_mod on 22.05.2018.
//

#ifndef VIRTUAL_MACHIEN_OBJECT_H
#define VIRTUAL_MACHIEN_OBJECT_H

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <values.h>

enum Object_Types  {
    OBJECT_FLAG,
    OBJECT_NUMBER,
    OBJECT_FLOAT,
    OBJECT_POINTER,
    OBJECT_C_POINTER
};

typedef uint32_t bitarr_t;

typedef struct Object{
    enum Object_Types type;
    size_t size;
    unsigned signage;
    union {
        bitarr_t value;
        double double_value;
        void* ptr;
    };
} object_t;

bitarr_t set_bit(bitarr_t target, uint8_t n){
    if (n > 63)
        exit(-10);

    return target | ((uint64_t)0x1 << n);
}

bitarr_t unset_bit(bitarr_t target, uint8_t n){
    if (n > 63)
        exit(-10);

    return target & (~((uint64_t)0x1 << n));
}

unsigned at_bit(bitarr_t target, uint8_t n){
    if(n > 63)
        exit(-10);
    return (target & ((uint64_t)0x1 << n)) >> n;
}

bitarr_t write_at(bitarr_t target, bitarr_t val, uint8_t n, uint8_t size){
    if (n > 63)
        exit(-10);
    for(uint8_t i = 0; i < size; i++){
        if(at_bit(val, i) == 0)
            target = unset_bit(target, n + i);
        else
            target = set_bit(target, n + i);
    }
    return target;
}

uint8_t get_last_significant_bit_index(bitarr_t target){
    for(uint8_t i = sizeof(bitarr_t) * CHAR_BIT - 1; i > 0; i--){
        if(target >> i != 0)
            return i;
    }
}

bool larger_then(object_t a, object_t b){
    if(a.signage == b.signage)
        return ((int32_t)(a.value - b.value) > 0) ? (true) : (false);
    else if(a.signage == 1)
        return false;
    return true;
}

object_t change_to_float_representation(object_t obj){
    if(obj.type != OBJECT_NUMBER)
        exit(-10);

    obj.value = write_at(obj.value, (uint32_t)obj.value, obj.size * CHAR_BIT / 2, sizeof(uint32_t) * CHAR_BIT);
    obj.type = OBJECT_FLOAT;
    return obj;
}

#endif //VIRTUAL_MACHIEN_OBJECT_H

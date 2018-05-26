//
// Created by a_mod on 22.05.2018.
//

#ifndef VIRTUAL_MACHIEN_OBJECT_H
#define VIRTUAL_MACHIEN_OBJECT_H

#include <stdint.h>
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
    uint8_t size;
    unsigned signage;
    union {
        bitarr_t value;
        double double_value;
        void* ptr;
    };
} object_t;

#endif //VIRTUAL_MACHIEN_OBJECT_H

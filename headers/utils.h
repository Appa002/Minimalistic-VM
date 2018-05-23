//
// Created by a_mod on 21.05.2018.
//

#ifndef VIRTUAL_MACHIEN_UTILS_H
#define VIRTUAL_MACHIEN_UTILS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

uint32_t read_n_byte_number(void *data, uint64_t size){
    uint8_t four_bytes_as_array[4] = {0x0, 0x0, 0x0, 0x0};

    if (size > 4)
        exit(-10);

    for (unsigned i = 0; i < size; i++) {
        four_bytes_as_array[3 - i] = *((uint8_t*) data + (size - i - 1));
    }

    return (four_bytes_as_array[0] << 32) | (four_bytes_as_array[1] << 16) |
           (four_bytes_as_array[2] << 8) | (four_bytes_as_array[3]);
}

void* unsign_n_byte_number(void *data, bool *is_negative){
    *(uint8_t*)data = *(uint8_t*)data & (uint8_t)127;

    if(is_negative == 0)
        return data;

    if ( *(uint8_t*)data >> (unsigned)7 == 1)
        *is_negative = true;
    else
        *is_negative = false;

    return data;
}

#endif //VIRTUAL_MACHIEN_UTILS_H

//
// Created by a_mod on 22.05.2018.
//

#ifndef VIRTUAL_MACHIEN_OBJECT_MATHS_H
#define VIRTUAL_MACHIEN_OBJECT_MATHS_H

#include <stdbool.h>

#include "object.h"

double underflow_save_subtract(double a, double b, unsigned* signage);

bool larger_then(object_t a, object_t b);

object_t add(object_t a, object_t b);

object_t subtract(object_t a, object_t b);

object_t multiply(object_t a, object_t b);

object_t divide(object_t a, object_t b);


#endif //VIRTUAL_MACHIEN_OBJECT_MATHS_H

#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

typedef struct color_t
{
    uint_fast8_t r;
    uint_fast8_t g;
    uint_fast8_t b;
    uint_fast8_t a;
} color_t;

#endif
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "rect.h"

typedef enum
{
    FLIP_NONE,
    FLIP_X,
    FLIP_Y,
    FLIP_BOTH
} flip_t;

typedef enum
{
    GRAPHICS_REGULAR
} graphics_type_t;

typedef struct graphics_data_regular_t
{
    int texture;
    rect_t src;
    rect_t dest;
    flip_t flip;
    double rotation;
} graphics_data_regular_t;

typedef union graphics_data_t
{
    graphics_data_regular_t regular;
} graphics_data_t;

typedef struct graphics_t
{
    graphics_type_t type;
    graphics_data_t data;
} graphics_t;

#endif
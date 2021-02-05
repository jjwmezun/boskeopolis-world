#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "color.h"
#include "layers.h"
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
    GRAPHICS_NULL,
    GRAPHICS_REGULAR,
    GRAPHICS_RECT
} graphics_type_t;

typedef struct graphics_data_regular_t
{
    int texture;
    rect_t src;
    rect_t dest;
    flip_t flip;
    double rotation;
} graphics_data_regular_t;

typedef struct graphics_data_rect_t
{
    rect_t dest;
    color_t color;
} graphics_data_rect_t;

typedef union graphics_data_t
{
    graphics_data_regular_t regular;
    graphics_data_rect_t rect;
} graphics_data_t;

typedef struct graphics_t
{
    graphics_type_t type;
    layer_t layer;
    graphics_data_t data;
} graphics_t;

graphics_t graphics_create_rect( layer_t layer, rect_t * rect, color_t * color );

#endif
#include "graphics.h"

graphics_t graphics_create_rect( layer_t layer, rect_t * rect, color_t * color )
{
    graphics_t graphics;
    graphics.type = GRAPHICS_RECT;
    graphics.layer = layer;
    graphics.data.rect.dest = *rect;
    graphics.data.rect.color = *color;
    return graphics;
};
#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "assoc_array.hpp"
#include "color.hpp"
#include "rect.hpp"
#include "text.hpp"

typedef struct
{
    Rect rect;
    Color color;
}
RectGraphics;

typedef struct
{
    unsigned int texture;
    unsigned int palette;
    Rect src;
    Rect dest;
    int flip_x;
    int flip_y;
    float rotation_x;
    float rotation_y;
    float rotation_z;
}
SpriteGraphics;

enum GFXType
{
    GFX_RECT,
    GFX_TEXT,
    GFX_SPRITE
};

typedef struct
{
    GFXType type;
    union
    {
        RectGraphics rect;
        Text text;
        SpriteGraphics sprite;
    } data;
}
Graphic;

Graphic graphic_create_rect( Rect rect, Color color );
Graphic graphic_create_full_rect( Color color );
Graphic graphic_create_text( Text text );
Graphic graphic_create_sprite
(
    unsigned int texture,
    unsigned int palette,
    Rect dest,
    float src_x,
    float src_y,
    AssocArray * args
);

#endif // GRAPHIC_H
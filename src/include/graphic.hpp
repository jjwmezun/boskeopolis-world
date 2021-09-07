#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "color.hpp"
#include "rect.hpp"
#include "text.hpp"
#include <variant>

struct RectGraphics
{
    Rect rect;
    Color color;
};

struct SpriteGraphics
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
};

struct TilemapGraphics
{
    unsigned int texture;
    unsigned int tilemap;
    unsigned int palette;
};

struct Graphic
{
    std::variant<std::monostate, RectGraphics, SpriteGraphics, TilemapGraphics, Text> data;
    int abs;

    static Graphic createRect( Rect rect, Color color );
    static Graphic createFullRect( Color color );
    static Graphic createSprite
    (
        unsigned int texture,
        unsigned int palette,
        Rect dest,
        float src_x,
        float src_y
    );
    static Graphic createText( Text text );
};

#endif // GRAPHIC_H
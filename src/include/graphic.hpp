#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "color.hpp"
#include "rect.hpp"

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

enum class GFXType
{
    RECT,
    SPRITE,
    TILEMAP
};

struct Graphic
{
    GFXType type;
    int abs;
    union
    {
        RectGraphics rect;
        SpriteGraphics sprite;
        TilemapGraphics tilemap;
    } data;

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
};

#endif // GRAPHIC_H
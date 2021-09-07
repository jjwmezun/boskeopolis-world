#include "graphic.hpp"
#include "render.hpp"
#include "unit.hpp"
#include <cstdlib>

Graphic Graphic::createRect( Rect rect, Color color )
{
    Graphic g;
    g.type = GFXType::RECT;
    g.abs = 0;
    g.data.rect.rect = rect;
    g.data.rect.color = color;
    return g;
};

Graphic Graphic::createFullRect( Color color )
{
    Rect rect = { 0.0, 0.0, ( float )( Unit::WINDOW_WIDTH_PIXELS ), ( float )( Unit::WINDOW_HEIGHT_PIXELS ) };
    return createRect( rect, color );
};

Graphic Graphic::createSprite
(
    unsigned int texture,
    unsigned int palette,
    Rect dest,
    float src_x,
    float src_y
)
{
    Graphic g;
    g.type = GFXType::SPRITE;
    g.abs = 0;
    g.data.sprite.texture = texture;
    g.data.sprite.palette = palette;
    g.data.sprite.dest = dest;
    g.data.sprite.src.x = src_x;
    g.data.sprite.src.y = src_y;
    g.data.sprite.src.w = dest.w;
    g.data.sprite.src.h = dest.h;
    g.data.sprite.flip_x = g.data.sprite.flip_y = 0;
    g.data.sprite.rotation_x = g.data.sprite.rotation_y = g.data.sprite.rotation_z = 0.0;
    return g;
};
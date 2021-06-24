#include "graphic.h"
#include "render.h"
#include "unit.h"

Graphic graphic_create_rect( Rect rect, Color color )
{
    Graphic g;
    g.type = GFX_RECT;
    g.data.rect.rect = rect;
    g.data.rect.color = color;
    return g;
};

Graphic graphic_create_full_rect( Color color )
{
    Rect rect = { 0.0, 0.0, ( float )( WINDOW_WIDTH_PIXELS ), ( float )( WINDOW_HEIGHT_PIXELS ) };
    return graphic_create_rect( rect, color );
};

Graphic graphic_create_text( Text text )
{
    Graphic g;
    g.type = GFX_TEXT;
    g.data.text = text;
    return g;
};

Graphic graphic_create_sprite
(
    unsigned int texture,
    unsigned int palette,
    Rect dest,
    float src_x,
    float src_y,
    AssocArray * args
)
{
    Graphic g;
    g.type = GFX_SPRITE;
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
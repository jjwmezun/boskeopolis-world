#include "graphic.hpp"
#include "render.hpp"
#include "unit.hpp"
#include <cstdlib>

Graphic Graphic::createRect( Rect rect, Color color )
{
    RectGraphics r { rect, color };
    return { r, 0 };
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
    SpriteGraphics g { texture, palette, { src_x, src_y, dest.w, dest.h }, dest, 0, 0, 0.0f, 0.0f, 0.0f };
    return { g, 0 };
};

Graphic Graphic::createText( Text text )
{
    return { text, 1 };
};
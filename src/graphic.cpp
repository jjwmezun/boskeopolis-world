#include "graphic.hpp"
#include "render.hpp"
#include "unit.hpp"
#include <cstdlib>

Graphic Graphic::createRect( Rect rect, Color color, bool abs )
{
    RectGraphics r { rect, color };
    return { r, abs };
};

Graphic Graphic::createFullRect( Color color, bool abs )
{
    Rect rect = { 0.0, 0.0, ( float )( Unit::WINDOW_WIDTH_PIXELS ), ( float )( Unit::WINDOW_HEIGHT_PIXELS ) };
    return createRect( rect, color, abs );
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
    return { g, false };
};

Graphic Graphic::createText( Text text, bool abs )
{
    return { text, abs };
};
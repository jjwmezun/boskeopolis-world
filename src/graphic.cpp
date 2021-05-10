#include "graphic.hpp"
#include "render.hpp"
#include "unit.hpp"

Graphic Graphic::createRect( Rect rect, Color color )
{
    Graphic g;
    g.type = Type::RECT;
    g.data.rect = { rect, color };
    return g;
};

Graphic Graphic::createFullRect( Color color )
{
    return createRect( { 0.0, 0.0, ( float )( Unit::WINDOW_WIDTH_PIXELS ), ( float )( Unit::WINDOW_HEIGHT_PIXELS ) }, color );
};

Graphic Graphic::createText( Text text )
{
    Graphic g;
    g.type = Type::TEXT;
    g.data.text = text;
    return g;
};
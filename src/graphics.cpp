#include "graphics.hpp"
#include "render.hpp"
#include "unit.hpp"

void Graphics::render()
{
    switch ( type )
    {
        case ( Type::RECT ):
        {
            Render::rect( data.rect.rect, data.rect.color );
        }
        break;
        case ( Type::TEXT ):
        {
            data.text.render();
        }
        break;
    }
};

Graphics Graphics::createRect( Rect rect, Color color )
{
    Graphics g;
    g.type = Type::RECT;
    g.data.rect = { rect, color };
    return g;
};

Graphics Graphics::createFullRect( Color color )
{
    return createRect( { 0.0, 0.0, ( float )( Unit::WINDOW_WIDTH_PIXELS ), ( float )( Unit::WINDOW_HEIGHT_PIXELS ) }, color );
};

Graphics Graphics::createText( Text text )
{
    Graphics g;
    g.type = Type::TEXT;
    g.data.text = text;
    return g;
};
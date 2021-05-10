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

Graphic Graphic::createSprite
(
    unsigned int texture,
    unsigned int palette,
    Rect dest,
    float src_x,
    float src_y,
    std::unordered_map<std::string, std::variant<bool, float>> args
)
{
    Graphic g;
    g.type = Type::SPRITE;
    g.data.sprite.texture = texture;
    g.data.sprite.palette = palette;
    g.data.sprite.dest = dest;
    g.data.sprite.src = { src_x, src_y, dest.w, dest.h };
    g.data.sprite.flip_x = g.data.sprite.flip_y = false;
    g.data.sprite.rotation_x = g.data.sprite.rotation_y = g.data.sprite.rotation_z = 0.0;
    return g;
};
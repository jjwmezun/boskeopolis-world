#include "graphic.hpp"
#include <stdexcept>
#include "render.hpp"

Graphic Graphic::createRect( Rect rect, Color color )
{
    Graphic g;
    g.type_ = Type::RECT;
    g.data_.rect.rect = rect;
    g.data_.rect.color = color;
    return g;
};

Graphic Graphic::createSprite( int texture_id, Rect src, Rect dest )
{
    Graphic g;
    g.type_ = Type::SPRITE;
    g.data_.sprite.texture_id = texture_id;
    g.data_.sprite.src = src;
    g.data_.sprite.dest = dest;
    return g;
};

RectGraphic & Graphic::getRect()
{
    if ( type_ != Type::RECT )
    {
        throw std::runtime_error( "Trying to call getRect on graphic that isn’t a rect." );
    }
    return data_.rect;
};

void Graphic::render() const
{
    switch ( type_ )
    {
        case ( Type::RECT ):
        {
            Render::rect( data_.rect );
        }
        break;
        case ( Type::SPRITE ):
        {
            Render::sprite( data_.sprite );
        }
        break;
    }
};

void Graphic::setId( int id )
{
    id_ = id;
};

int Graphic::getId() const
{
    return id_;
};
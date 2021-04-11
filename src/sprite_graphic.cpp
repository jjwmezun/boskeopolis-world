#include "render.hpp"
#include "sprite_graphic.hpp"

SpriteGraphic::SpriteGraphic( int texture_id, Rect src, Rect dest )
:
    texture_id_ ( texture_id ),
    src_ ( src ),
    dest_ ( dest )
{};

void SpriteGraphic::render() const
{
    Render::sprite( texture_id_, src_, dest_ );
};
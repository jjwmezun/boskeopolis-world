#include "render.hpp"
#include "sprite_graphic.hpp"

SpriteGraphic::SpriteGraphic( unsigned int texture_id, unsigned int palette_id, Rect src, Rect dest, bool flip_x, bool flip_y )
:
    texture_id_ ( texture_id ),
    palette_id_ ( palette_id ),
    src_ ( src ),
    dest_ ( dest ),
    flip_x_ ( flip_x ),
    flip_y_ ( flip_y )
{};

void SpriteGraphic::render() const
{
    Render::sprite( texture_id_, palette_id_, src_, dest_, flip_x_, flip_y_ );
};
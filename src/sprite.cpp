#include "sprite.hpp"

bool Sprite::hasType( SpriteType type ) const
{
    for ( const auto & t : types )
    {
        if ( t == type )
        {
            return true;
        }
    }
    return false;
};
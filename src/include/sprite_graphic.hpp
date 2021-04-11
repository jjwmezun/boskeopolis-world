#ifndef SPRITE_GRAPHIC_H
#define SPRITE_GRAPHIC_H

#include "rect.hpp"

struct SpriteGraphic
{
    unsigned int texture_id;
    Rect src;
    Rect dest;
};

#endif // SPRITE_GRAPHIC_H
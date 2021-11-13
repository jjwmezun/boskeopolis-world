#ifndef HERO_H
#define HERO_H

#include "sprite.hpp"

namespace Hero
{
    Sprite create();
    void mapInteract( Sprite & self, LevelState & level );
};

#endif // HERO_H
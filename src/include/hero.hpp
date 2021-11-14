#ifndef HERO_H
#define HERO_H

#include "sprite.hpp"

namespace Hero
{
    Sprite create();
    void mapInteract( Sprite & self, LevelState & level );
    bool testOnGround( const Sprite & self, const LevelState & level );
    void moveHorizontally( Sprite & self, float start_speed, float top_speed, float traction );
};

#endif // HERO_H
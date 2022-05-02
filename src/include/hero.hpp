#ifndef HERO_H
#define HERO_H

#include "sprite.hpp"

namespace Hero
{
    Sprite create( unsigned int state );
    void mapInteract( Sprite & self, LevelState & level );
    void updateGraphics( Sprite & self, LevelState & level, int invincibility );
    bool testOnGround( const Sprite & self, const LevelState & level );
    void moveHorizontally( Sprite & self, float start_speed, float top_speed, float traction );
};

#endif // HERO_H
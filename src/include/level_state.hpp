#ifndef LEVEL_STATE_H
#define LEVEL_STATE_H

#include "map.hpp"
#include "sprite.hpp"

struct LevelState
{
    LevelState();
    void update();
    void init( unsigned int state );

    Map map;
    Sprite hero;
};

#endif // LEVEL_STATE_H
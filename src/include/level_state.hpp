#ifndef LEVEL_STATE_H
#define LEVEL_STATE_H

#include "map.hpp"
#include "sprite.hpp"

class LevelState
{
    public:
        LevelState();
        void update();
        void init( unsigned int state );

    private:
        Map map_;
        Sprite hero_;
};

#endif // LEVEL_STATE_H
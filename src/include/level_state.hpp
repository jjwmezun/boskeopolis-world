#ifndef LEVEL_STATE_H
#define LEVEL_STATE_H

#include "sprite.hpp"

struct LevelState
{
    public:
        LevelState();
        void update();
        void init( unsigned int state );

    private:
        Sprite hero_;
};

#endif // LEVEL_STATE_H
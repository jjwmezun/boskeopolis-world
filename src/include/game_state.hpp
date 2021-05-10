#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "state_graphics.hpp"

struct GameState
{
    enum class Type
    {
        TITLE,
        LEVEL
    };

    Type type;
    StateGraphics graphics;
};

#endif // GAME_STATE_H
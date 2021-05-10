#ifndef GAME_STATE_MACHINE
#define GAME_STATE_MACHINE

#include "game_state.hpp"

namespace GameStateMachine
{
    void init();
    void update();
    void render();
    void popState();
    void changeStateTitle();
    void changeStateLevel();
};

#endif // GAME_STATE_MACHINE
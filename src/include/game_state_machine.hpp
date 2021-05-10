#ifndef GAME_STATE_MACHINE
#define GAME_STATE_MACHINE

#include "game_state.hpp"

namespace GameStateMachine
{
    static constexpr int MAX_STATES = 5;

    void init();
    void update();
    void changeState( GameState state );
    void pushState( GameState state );
    void popState();

    GameState createTitleState();
    GameState createLevelState();
    GameState createPauseState();
};

#endif // GAME_STATE_MACHINE
#ifndef GAME_STATE_MACHINE
#define GAME_STATE_MACHINE

class GameState;

namespace GameStateMachine
{
    void init();
    void update();
    void render();
    void pushState( GameState state );
    void changeState( GameState state );
    void popState();
};

#endif // GAME_STATE_MACHINE
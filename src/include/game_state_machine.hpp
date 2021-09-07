#ifndef GAME_STATE_MACHINE_H
#define GAME_STATE_MACHINE_H

#include "level_state.hpp"
#include <string>
#include "title_state.hpp"
#include <variant>

typedef std::variant<std::monostate, TitleState, LevelState> GameState;

namespace GameStateMachine
{
    void init();
    void update();
    void changeState( GameState state );
};

#endif // GAME_STATE_MACHINE_H
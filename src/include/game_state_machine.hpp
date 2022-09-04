#ifndef GAME_STATE_MACHINE_H
#define GAME_STATE_MACHINE_H

#include "level_state.hpp"
#include <string>
#include "title_state.hpp"
#include "treasure_list_state.hpp"
#include "treasure_message_state.hpp"
#include <variant>

typedef std::variant<std::monostate, TitleState, LevelState, TreasureMessageState, TreasureListState> GameState;

namespace GameStateMachine
{
    void init();
    void update();
    void changeState( GameState state );
    void pushState( GameState state );
    void popState();
};

#endif // GAME_STATE_MACHINE_H
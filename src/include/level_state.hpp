#ifndef LEVEL_STATE_H
#define LEVEL_STATE_H

#include "game_state.hpp"

class LevelState : public GameState
{
    public:
        void init() override;
        void update( bool top_state ) override;
};

#endif // LEVEL_STATE_H
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "state_graphics.hpp"

class GameState
{
    public:
        virtual ~GameState();
        virtual void update( bool top_state ) = 0;
        virtual void init();
        virtual void close();
        void render() const;

    protected:
        StateGraphics graphics;
};

#endif // GAME_STATE_H
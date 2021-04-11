#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "state_graphics.hpp"

struct GameStateTitle
{
};

class GameState
{
    public:
        static GameState createTitle();
        static GameState createLevel();
        void update( bool top_state );
        void init();
        void close();
        void render() const;

    private:
        enum class Type
        {
            TITLE,
            LEVEL
        };
        Type type_;
        union {
            GameStateTitle title;
        } data_;
        StateGraphics graphics;
};

#endif // GAME_STATE_H
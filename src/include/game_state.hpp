#ifndef GAME_STATE_H
#define GAME_STATE_H

struct GameState
{
    enum class Type
    {
        TITLE,
        LEVEL,
        PAUSE
    };

    Type type;
};

#endif // GAME_STATE_H
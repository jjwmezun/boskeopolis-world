#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "rect.hpp"

#define MAX_SPRITE_PROPS 32

typedef struct
{
    unsigned int gfx;
    Rect position;
    float accx;
    float accy;
    float vx;
    float vy;
    float top_speed;
}
Sprite;

typedef struct
{
    Sprite hero;
}
LevelState;

typedef struct
{
    enum class Type
    {
        TITLE,
        LEVEL,
        PAUSE
    };

    Type type;
    union
    {
        LevelState level;
    } data;
}
GameState;

#endif // GAME_STATE_H
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "rect.hpp"

enum class Direction
{
    LEFT,
    RIGHT
};

struct Sprite
{
    unsigned int gfx;
    Rect position;
    float accx;
    float accy;
    float vx;
    float vy;
    float top_speed;
    Direction dir;
};

struct GameState
{
    enum class Type
    {
        TITLE,
        LEVEL,
        PAUSE
    };

    Type type;
    Sprite hero;
};

#endif // GAME_STATE_H
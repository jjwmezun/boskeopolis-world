#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "rect.hpp"
#include <string>
#include <unordered_map>
#include <variant>

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
    std::unordered_map<std::string, int_fast16_t> * props;
};

struct LevelState
{
    Sprite hero;
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
    union
    {
        LevelState level;
    } data;
};

#endif // GAME_STATE_H
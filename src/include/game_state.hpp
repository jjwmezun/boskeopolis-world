#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "bytecode_machine.hpp"
#include "rect.hpp"
#include <string>
#include <unordered_map>
#include <variant>

#define MAX_SPRITE_PROPS 32

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
    VMValue props[ MAX_SPRITE_PROPS ];
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
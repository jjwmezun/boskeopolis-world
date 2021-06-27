#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "rect.h"
#include "sprite.h"

#define MAX_SPRITE_PROPS 32

typedef struct
{
    Sprite hero;
}
LevelState;


typedef enum
{
    STATE_TITLE,
    STATE_LEVEL,
    STATE_PAUSE
}
StateType;

typedef struct
{
    StateType type;
    union
    {
        LevelState level;
    } data;
}
GameState;

#endif // GAME_STATE_H
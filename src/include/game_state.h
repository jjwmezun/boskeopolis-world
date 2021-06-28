#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "map.h"
#include "rect.h"
#include "sprite.h"

#define MAX_SPRITE_PROPS 32

typedef struct
{
    Sprite hero;
    Map map;
}
LevelState;

typedef struct
{
    const char * msg;
}
ErrorState;

typedef enum
{
    STATE_TITLE,
    STATE_LEVEL,
    STATE_PAUSE,
    STATE_ERROR
}
StateType;

typedef struct
{
    StateType type;
    union
    {
        LevelState level;
        ErrorState error;
    } data;
}
GameState;

#endif // GAME_STATE_H
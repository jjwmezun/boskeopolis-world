#ifndef SPRITE_H
#define SPRITE_H

#include <functional>
#include "prop_map.hpp"
#include "rect.hpp"
#include <vector>

class LevelState;
class Sprite;

typedef std::function<void( Sprite & self, LevelState & level )> SpriteUpdate;
typedef std::function<void( Sprite & self, Sprite & other, LevelState & level )> SpriteInteract;

enum class SpriteType
{
    HERO,
    HARMFUL,
    BUMPS_EACH_OTHER
};

enum class SpriteState
{
    NORMAL,
    JUMPING,
    ON_LADDER
};

struct Sprite
{
    Rect position;
    unsigned int gfx;
    float vx;
    float accx;
    float top_speed;
    float vy;
    float accy;
    float gravity;
    bool jump_lock;
    SpriteState state;
    SpriteUpdate update = []( Sprite & self, LevelState & level ){};
    SpriteInteract interact = []( Sprite & self, Sprite & other, LevelState & level ){};
    PropMap props;
    std::vector<SpriteType> types;

    bool hasType( SpriteType type ) const;
};

#endif // SPRITE_H
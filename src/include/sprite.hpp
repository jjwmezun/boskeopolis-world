#ifndef SPRITE_H
#define SPRITE_H

#include <functional>
#include "rect.hpp"
#include <unordered_map>
#include <variant>

class LevelState;
class Sprite;

typedef std::function<void( Sprite & self, LevelState & level )> SpriteUpdate;

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
    SpriteUpdate update = []( Sprite & self, LevelState & level ){};
    std::unordered_map<std::string, std::variant<bool, int, float, std::string>> props;
};

#endif // SPRITE_H
#ifndef LEVEL_STATE_H
#define LEVEL_STATE_H

#include "inventory.hpp"
#include "layer_rain.hpp"
#include "map.hpp"
#include "sprite.hpp"
#include "tileset_system.hpp"
#include "unit.hpp"
#include <vector>

struct LevelState
{
    LevelState();
    void update();
    void init( unsigned int state );
    Sprite & getHero();

    std::vector<Sprite> sprites;
    Map map;
    Inventory inventory;
    TilesetSystem tilesets;
    LayerRain rain;
};

#endif // LEVEL_STATE_H
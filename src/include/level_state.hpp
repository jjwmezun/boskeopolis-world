#ifndef LEVEL_STATE_H
#define LEVEL_STATE_H

#include "inventory.hpp"
#include "layer_rain.hpp"
#include "map.hpp"
#include "sprite.hpp"
#include "tileset_system.hpp"
#include "unit.hpp"

struct LevelState
{
    LevelState();
    void update();
    void init( unsigned int state );

    Map map;
    Sprite hero;
    Inventory inventory;
    TilesetSystem tilesets;
    LayerRain rain;
};

#endif // LEVEL_STATE_H
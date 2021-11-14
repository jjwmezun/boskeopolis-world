#ifndef LEVEL_STATE_H
#define LEVEL_STATE_H

#include "inventory.hpp"
#include "map.hpp"
#include "sprite.hpp"
#include "tileset_system.hpp"

struct LevelState
{
    LevelState();
    void update();
    void init( unsigned int state );

    Map map;
    Sprite hero;
    Inventory inventory;
    TilesetSystem tilesets;
};

#endif // LEVEL_STATE_H
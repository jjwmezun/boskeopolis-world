#ifndef MAP_H
#define MAP_H

#include "color.hpp"
#include <list>
#include "prop_map.hpp"
#include <string>
#include <vector>

class LevelState;
class Sprite;
class TilesetSystem;

enum class MapObjType
{
    GEM,
    TREASURE
};

struct MapObj
{
    MapObjType type;
    unsigned int tilemap;
    PropMap props;
};

struct MapSprite
{
    unsigned int type;
    unsigned int x;
    unsigned int y;
};

struct MapLayer
{
    enum class Type
    {
        COLLISION,
        TILES,
        OBJ,
        SPRITES,
        __NULL
    };

    int width;
    int height;
    Type type = Type::__NULL;
    std::vector<int> tiles;
    PropMap props;
};

enum class MapCollisionType
{
    SOLID = 0,
    SOLID_TOP = 1,
    CLIMB = 2,
    __NULL
};

struct Map
{
    Map( std::string slug );
    void init( unsigned int state, TilesetSystem & tilesets );
    int getIFromXAndY( int x, int y ) const;
    bool testTile( int i, MapCollisionType type ) const;
    bool testTileMulti( int i, std::vector<MapCollisionType> types ) const;

    int width;
    int height;
    std::vector<MapLayer> layers;
    std::vector<std::vector<MapCollisionType>> collision;
    std::vector<std::vector<MapObj>> objs;
    std::vector<MapSprite> sprites;
    Color bg_color;
};

#endif // MAP_H
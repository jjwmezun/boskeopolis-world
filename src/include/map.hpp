#ifndef MAP_H
#define MAP_H

#include "color.hpp"
#include "prop_map.hpp"
#include <string>
#include <vector>

class LevelState;
class Sprite;

enum class MapObjType
{
    GEM
};

struct MapObj
{
    MapObjType type;
    unsigned int tilemap;
    PropMap props;
};

struct MapLayer
{
    enum class Type
    {
        TILES,
        OBJ,
        __NULL
    };

    int width;
    int height;
    Type type = Type::__NULL;
    std::vector<int> tiles;
    PropMap props;
};

struct Map
{
    Map( std::string slug );
    void init( unsigned int state );

    int width;
    int height;
    std::vector<MapLayer> layers;
    std::vector<std::vector<MapObj>> objs;
    Color bg_color;
};

#endif // MAP_H
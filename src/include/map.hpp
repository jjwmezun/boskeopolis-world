#ifndef MAP_H
#define MAP_H

#include "color.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>

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
    std::unordered_map<std::string, std::variant<bool, int, float, std::string>> props;
};

struct Map
{
    Map( std::string slug );
    void init( unsigned int state );

    int width;
    int height;
    std::vector<MapLayer> layers;
    Color bg_color;
};

#endif // MAP_H
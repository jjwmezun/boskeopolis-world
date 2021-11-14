#ifndef TILESET_SYSTEM_H
#define TILESET_SYSTEM_H

#include <string>
#include "tileset.hpp"
#include <unordered_map>

class TilesetSystem
{
    public:
        void init();
        Tileset get( std::string name );

    private:
        std::unordered_map<std::string, Tileset> tilesets_;
};

#endif // TILESET_SYSTEM_H
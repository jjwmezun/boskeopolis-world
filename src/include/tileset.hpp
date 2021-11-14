#ifndef TILESET_H
#define TILESET_H

#include <string>
#include "tile.hpp"
#include <unordered_map>

class Tileset
{
    public:
        Tileset( std::string type );
        Tile get( int n ) const;

    private:
        std::unordered_map<int, Tile> tiles_;
};

#endif // TILESET_H
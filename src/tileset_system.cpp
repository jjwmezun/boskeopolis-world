#include "tileset_system.hpp"

void TilesetSystem::init()
{
    tilesets_.insert( std::pair<std::string, Tileset> ( "objects", { "objects" } ) );
};

Tileset TilesetSystem::get( std::string name )
{
    const auto tileset = tilesets_.find( name );
    if (tileset == tilesets_.end()) {
        tilesets_.insert( std::pair<std::string, Tileset> ( name, { name } ) );
        const auto tileset = tilesets_.find( name );
        return tileset->second;
    }
    return tileset->second;
};
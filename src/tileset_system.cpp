#include "tileset_system.hpp"

void TilesetSystem::init()
{
    tilesets_.insert( std::pair<std::string, Tileset> ( "objects", { "objects" } ) );
};

Tileset TilesetSystem::get( std::string name )
{
    return tilesets_.find( "objects" )->second;
};
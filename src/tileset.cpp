#include <cmath>
#include "io.hpp"
#include "jjson.hpp"
#include "log.hpp"
#include "tileset.hpp"
#include <vector>

struct TileData {
    int id = -1;
    int x = -1;
    int y = -1;
    int animation = 0;
    int palette = 0;
};

Tileset::Tileset( std::string type )
{
    json_value * root = JJSON::load( "assets/tilesets/" + type + ".json", "Could not parse JSON for tileset “" + type + "”." );
    if ( root == nullptr )
    {
        return;
    }

    std::vector<TileData> tile_data_list;
    int image_width = -1;
    int tile_width = -1;
    for ( unsigned int i = 0; i < root->u.object.length; ++i )
    {
        const json_object_entry root_entry = root->u.object.values[ i ];
        if ( strcmp( "tiles", root_entry.name ) == 0 )
        {
            if ( root_entry.value->type != json_array )
            {
                Log::sendError( "JSON for tileset “" + type + "” is malformed: “tiles” isn’t an array." );
            }
            const auto & tiles = root_entry.value->u.array;
            for ( unsigned int i = 0; i < tiles.length; ++i )
            {
                TileData tile_data;
                if ( tiles.values[ i ]->type != json_object )
                {
                    Log::sendError( "JSON for tileset “" + type + "” is malformed: tile isn’t an object." );
                }
                const auto & tile = tiles.values[ i ]->u.object;
                for ( unsigned int j = 0; j < tile.length; ++j )
                {
                    const auto & tile_entry = tile.values[ j ];
                    if ( strcmp( "id", tile_entry.name ) == 0 )
                    {
                        if ( tile_entry.value->type != json_integer )
                        {
                            Log::sendError( "JSON for tileset “" + type + "” is malformed: tileset id isn’t an integer." );
                        }
                        tile_data.id = tile_entry.value->u.integer;
                    }
                    else if ( strcmp( "properties", tile_entry.name ) == 0 )
                    {
                        if ( tile_entry.value->type != json_array )
                        {
                            Log::sendError( "JSON for tileset “" + type + "” is malformed: tileset properties isn’t an array." );
                        }
                        const auto & props = tile_entry.value->u.array;
                        for ( unsigned int propi = 0; propi < props.length; ++propi )
                        {
                            if ( props.values[ propi ]->type != json_object )
                            {
                                Log::sendError( "JSON for tileset “" + type + "” is malformed: tile property isn’t an object." );
                            }
                            const auto & prop = props.values[ propi ]->u.object;
                            std::string name = "";
                            int value = -1;
                            for ( unsigned int propj = 0; propj < prop.length; ++propj )
                            {
                                const auto & prop_entry = prop.values[ propj ];
                                if ( strcmp( "name", prop_entry.name ) == 0 )
                                {
                                    if ( prop_entry.value->type != json_string )
                                    {
                                        Log::sendError( "JSON for tileset “" + type + "” is malformed: tileset property name isn’t an integer." );
                                    }
                                    name = prop_entry.value->u.string.ptr;
                                }
                                else if ( strcmp( "value", prop_entry.name ) == 0 )
                                {
                                    if ( prop_entry.value->type != json_integer )
                                    {
                                        Log::sendError( "JSON for tileset “" + type + "” is malformed: tileset property value isn’t an integer." );
                                    }
                                    value = prop_entry.value->u.integer;
                                }
                            }
                            if ( name == "x" )
                            {
                                tile_data.x = value;
                            }
                            else if ( name == "y" )
                            {
                                tile_data.y = value;
                            }
                            else if ( name == "animation" )
                            {
                                tile_data.animation = value;
                            }
                            else if ( name == "palette" )
                            {
                                tile_data.palette = value;
                            }
                        }
                    }
                    else if ( strcmp( "y", tile_entry.name ) == 0 )
                    {
                        if ( tile_entry.value->type != json_integer )
                        {
                            Log::sendError( "JSON for tileset “" + type + "” is malformed: tileset y isn’t an integer." );
                        }
                        tile_data.y = tile_entry.value->u.integer;
                    }
                    else if ( strcmp( "animation", tile_entry.name ) == 0 )
                    {
                        if ( tile_entry.value->type != json_integer )
                        {
                            Log::sendError( "JSON for tileset “" + type + "” is malformed: tileset animation isn’t an integer." );
                        }
                        tile_data.animation = tile_entry.value->u.integer;
                    }
                    else if ( strcmp( "palette", tile_entry.name ) == 0 )
                    {
                        if ( tile_entry.value->type != json_integer )
                        {
                            Log::sendError( "JSON for tileset “" + type + "” is malformed: tileset palette isn’t an integer." );
                        }
                        tile_data.palette = tile_entry.value->u.integer;
                    }
                }

                tile_data_list.push_back( tile_data );
            }
        }
        else if ( strcmp( "imagewidth", root_entry.name ) == 0 )
        {
            if ( root_entry.value->type != json_integer )
            {
                Log::sendError( "JSON for tileset “" + type + "” is malformed: “imagewidth” isn’t an integer." );
            }
            image_width = root_entry.value->u.integer;
        }
        else if ( strcmp( "tilewidth", root_entry.name ) == 0 )
        {
            if ( root_entry.value->type != json_integer )
            {
                Log::sendError( "JSON for tileset “" + type + "” is malformed: “tilewidth” isn’t an integer." );
            }
            tile_width = root_entry.value->u.integer;
        }
    }

    json_value_free( root );

    const int tileset_width = std::floor( ( double )( image_width ) / ( double )( tile_width ) );
    for ( auto & tile : tile_data_list )
    {
        if ( tile.x == -1 ) {
            tile.x = tile.id % tileset_width;
        }
        if ( tile.y == -1 ) {
            tile.y = ( int )( std::floor( ( double )( tile.id ) / ( double )( tileset_width ) ) );
        }
        tiles_.insert( std::pair<int, Tile>
        (
            tile.id,
            {
                ( unsigned char )( tile.x ),
                ( unsigned char )( tile.y ),
                ( unsigned char )( tile.palette ),
                ( unsigned char )( tile.animation )
            }
        ));
    }
};

Tile Tileset::get( int n ) const
{
    const auto seek = tiles_.find( n );
    return ( seek == tiles_.end() ) ? Tile{ 255, 255, 255, 255 } : seek->second;
};
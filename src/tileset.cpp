#include "io.hpp"
#include "jjson.hpp"
#include "log.hpp"
#include "tileset.hpp"

Tileset::Tileset( std::string type )
{
    json_value * root = JJSON::load( "assets/tilesets/" + type + ".json", "Could not parse JSON for tileset “" + type + "”." );
    if ( root == nullptr )
    {
        return;
    }

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
                int id = -1;
                int x = -1;
                int y = -1;
                int animation = 0;
                int palette = 0;
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
                        id = tile_entry.value->u.integer;
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
                                x = value;
                            }
                            else if ( name == "y" )
                            {
                                y = value;
                            }
                            else if ( name == "animation" )
                            {
                                animation = value;
                            }
                            else if ( name == "palette" )
                            {
                                palette = value;
                            }
                        }
                    }
                    else if ( strcmp( "y", tile_entry.name ) == 0 )
                    {
                        if ( tile_entry.value->type != json_integer )
                        {
                            Log::sendError( "JSON for tileset “" + type + "” is malformed: tileset y isn’t an integer." );
                        }
                        y = tile_entry.value->u.integer;
                    }
                    else if ( strcmp( "animation", tile_entry.name ) == 0 )
                    {
                        if ( tile_entry.value->type != json_integer )
                        {
                            Log::sendError( "JSON for tileset “" + type + "” is malformed: tileset animation isn’t an integer." );
                        }
                        animation = tile_entry.value->u.integer;
                    }
                    else if ( strcmp( "palette", tile_entry.name ) == 0 )
                    {
                        if ( tile_entry.value->type != json_integer )
                        {
                            Log::sendError( "JSON for tileset “" + type + "” is malformed: tileset palette isn’t an integer." );
                        }
                        palette = tile_entry.value->u.integer;
                    }
                }

                if ( x >= 0 && y >= 0 )
                {
                    tiles_.insert( std::pair<int, Tile>
                    (
                        id,
                        {
                            ( unsigned char )( x ),
                            ( unsigned char )( y ),
                            ( unsigned char )( palette ),
                            ( unsigned char )( animation )
                        }
                    ));
                }
            }
        }
    }

    json_value_free( root );
};

Tile Tileset::get( int n ) const
{
    const auto seek = tiles_.find( n );
    return ( seek == tiles_.end() ) ? Tile{ 255, 255, 255, 255 } : seek->second;
};
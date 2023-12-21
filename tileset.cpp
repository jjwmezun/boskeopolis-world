#include <cmath>
#include "json.hpp"
#include <stdexcept>
#include "tileset.hpp"

namespace BSW
{
    void Tileset::init()
    {
        const JSON json { "assets/tilesets/" + name_ + ".json" };
        const int width = json.getInt( "tilewidth" );
        const JSONArray tiles = json.getArray( "tiles" );
        tiles.forEach( [&]( JSONItem item )
        {
            const JSONObject o = item.asObject();
            int id = o.getInt( "id" );
            unsigned char animation = 0;
            unsigned char x = id % width;
            unsigned char y = std::floor( id / width );
            unsigned char palette = 0;
            std::string blocktype = "none";
            ArgList args {};
            const JSONArray props = o.getArray( "properties" );
            props.forEach( [&]( JSONItem item )
            {
                const JSONObject o = item.asObject();
                const std::string name = o.getString( "name" );
                const std::string type = o.getString( "type" );
                if ( name == "animation" )
                {
                    if ( type != "int" )
                    {
                        throw std::runtime_error( "Tileset " + name_ + " has an invalid format: a tile’s animation isn’t an integer." );
                    }
                    animation = ( unsigned char )( o.getInt( "value" ) );
                }
                else if ( name == "palette" )
                {
                    if ( type != "int" )
                    {
                        throw std::runtime_error( "Tileset " + name_ + " has an invalid format: a tile’s palette isn’t an integer." );
                    }
                    palette = ( unsigned char )( o.getInt( "value" ) );
                }
                else if ( name == "x" )
                {
                    if ( type != "int" )
                    {
                        throw std::runtime_error( "Tileset " + name_ + " has an invalid format: a x isn’t an integer." );
                    }
                    x = ( unsigned char )( o.getInt( "value" ) );
                }
                else if ( name == "y" )
                {
                    if ( type != "int" )
                    {
                        throw std::runtime_error( "Tileset " + name_ + " has an invalid format: a tile’s y isn’t an integer." );
                    }
                    y = ( unsigned char )( o.getInt( "value" ) );
                }
                else if ( name == "type" )
                {
                    if ( type != "string" )
                    {
                        throw std::runtime_error( "Tileset " + name_ + " has an invalid format: a tile’s type animation isn’t a string." );
                    }
                    blocktype = o.getString( "value" );
                }
                else
                {
                    if ( type == "int" )
                    {
                        args.insert({ name, o.getInt( "value" ) });
                    }
                    else if ( type == "string" )
                    {
                        args.insert({ name, o.getString( "value" ) });
                    }
                }
            });

            if ( blocktype != "none" )
            {
                behavior_info_.insert( std::pair<unsigned int, BehaviorInfo> ( id, { blocktype, args } ) );
            }

            tiles_.insert( std::pair<unsigned int, Tile> ( id, { x, y, animation, palette } ) );
        });
    }

    const std::optional<Tile> Tileset::getTile( unsigned int type ) const
    {
        const auto it = tiles_.find( type );
        if ( it == tiles_.end() )
        {
            return std::nullopt;
        }
        return std::optional<Tile> ( it->second );
    };

    const std::optional<BehaviorInfo> Tileset::getBehaviorInfo( unsigned int type ) const
    {
        const auto it = behavior_info_.find( type );
        if ( it == behavior_info_.end() )
        {
            return std::nullopt;
        }
        return std::optional<BehaviorInfo> ( it->second );
    };
}
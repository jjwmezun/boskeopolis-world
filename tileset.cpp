#include "arg.hpp"
#include <cmath>
#include "json.hpp"
#include <stdexcept>
#include "tileset.hpp"

namespace BSW
{
    typedef std::function<std::vector<BlockBehavior>( const ArgList & args )> BlockBehaviorGenerator;

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

            while ( data_.size() <= static_cast<size_t>( id ) )
            {
                data_.push_back({});
            }

            std::unordered_map<std::string, BlockBehaviorGenerator> generators =
            {
                {
                    "money",
                    [&]( const ArgList & args )
                    {
                        BlockCondition cond { BlockCondition::Type::PROTAG, args };
                        return std::vector<BlockBehavior>{
                            { { BlockComponent::Type::MONEY, args }, { cond } },
                            { { BlockComponent::Type::REMOVE, args }, { cond } }
                        };
                    }
                }
            };

            auto it = generators.find( blocktype );
            const std::vector<BlockBehavior> behaviors = ( it != generators.end() )
                ? it->second( args )
                : std::vector<BlockBehavior>{};

            data_[ id ] = { x, y, animation, palette, behaviors };
        });
    }

    const BlockType & Tileset::getBlockType( unsigned int type ) const
    {
        if ( data_.at( type ).isNull() )
        {
            throw std::runtime_error( "Invalid block type #" + std::to_string( type ) );
        }
        return data_.at( type );
    };
}
#include <cmath>
#include "graphic.hpp"
#include "jjson.hpp"
#include "log.hpp"
#include "map.hpp"
#include "render.hpp"
#include <sstream>
#include "tile.hpp"

Map::Map( std::string slug )
:
    width ( -1 ),
    height ( -1 ),
    bg_color ( { 0.0f, 0.0f, 0.0f, 255.0f } )
{
    json_value * root = JJSON::load( "assets/maps/" + slug + ".json", "Could not parse JSON for map “" + slug + "”." );
    if ( root == nullptr )
    {
        return;
    }

    for ( unsigned int i = 0; i < root->u.object.length; ++i )
    {
        const json_object_entry root_entry = root->u.object.values[ i ];
        if ( strcmp( "width", root_entry.name ) == 0 )
        {
            if ( root_entry.value->type != json_integer )
            {
                Log::sendError( "JSON for map “" + slug + "” is malformed: “width” isn’t an integer." );
            }

            width = root_entry.value->u.integer;
        }
        else if ( strcmp( "height", root_entry.name ) == 0 )
        {
            if ( root_entry.value->type != json_integer )
            {
                Log::sendError( "JSON for map “" + slug + "” is malformed: “height” isn’t an integer." );
            }

            height = root_entry.value->u.integer;
        }
        if ( strcmp( "backgroundcolor", root_entry.name ) == 0 )
        {
            if ( root_entry.value->type != json_string )
            {
                Log::sendError( "JSON for map “" + slug + "” is malformed: “backgroundcolor” isn’t a string." );
            }

            // Convert string o’ hex bytes into decimal floats & set corresponding property on bg_color.
            std::string bgc = &root_entry.value->u.string.ptr[ 1 ];
            float * rgb[ 3 ] = { &bg_color.r, &bg_color.g, &bg_color.b };
            for ( int i = 0; i < 3; ++i )
            {
                int n;
                std::string s = bgc.substr( 2 * i, 2 );
                std::istringstream( s ) >> std::hex >> n;
                *rgb[ i ] = float( n );
            }
        }
        if ( strcmp( "layers", root_entry.name ) == 0 )
        {
            if ( root_entry.value->type != json_array )
            {
                Log::sendError( "JSON for map “" + slug + "” is malformed: “layers” isn’t an array." );
            }

            const auto & layer = root_entry.value->u.array;
            for ( unsigned int i = 0; i < layer.length; ++i )
            {
                MapLayer l;
                if ( layer.values[ i ]->type != json_object )
                {
                    Log::sendError( "JSON for map “" + slug + "” is malformed: layer isn’t an object." );
                }
                const auto & lobj = layer.values[ i ]->u.object;
                for ( unsigned int j = 0; j < lobj.length; ++j )
                {
                    const auto & layer_entry = lobj.values[ j ];
                    if ( strcmp( "name", layer_entry.name ) == 0 )
                    {
                        if ( layer_entry.value->type != json_string )
                        {
                            Log::sendError( "JSON for map “" + slug + "” is malformed: layer name isn’t a string." );
                        }
                        if ( strcmp( "tiles", layer_entry.value->u.string.ptr ) == 0 )
                        {
                            l.type = MapLayer::Type::TILES;
                        }
                        else if ( strcmp( "obj", layer_entry.value->u.string.ptr ) == 0 )
                        {
                            l.type = MapLayer::Type::OBJ;
                        }
                    }
                    else if ( strcmp( "width", layer_entry.name ) == 0 )
                    {
                        if ( layer_entry.value->type != json_integer )
                        {
                            Log::sendError( "JSON for map “" + slug + "” is malformed: layer width isn’t an int." );
                        }
                        l.width = layer_entry.value->u.integer;
                    }
                    else if ( strcmp( "height", layer_entry.name ) == 0 )
                    {
                        if ( layer_entry.value->type != json_integer )
                        {
                            Log::sendError( "JSON for map “" + slug + "” is malformed: layer height isn’t an int." );
                        }
                        l.height = layer_entry.value->u.integer;
                    }
                    else if ( strcmp( "data", layer_entry.name ) == 0 )
                    {
                        if ( layer_entry.value->type != json_array )
                        {
                            Log::sendError( "JSON for map “" + slug + "” is malformed: layer data isn’t an array." );
                        }

                        for ( unsigned int k = 0; k < layer_entry.value->u.array.length; ++k )
                        {
                            const auto & tile = layer_entry.value->u.array.values[ k ];
                            if ( tile->type != json_integer )
                            {
                                Log::sendError( "JSON for map “" + slug + "” is malformed: layer data tile isn’t an int." );
                            }
                            l.tiles.push_back( tile->u.integer );
                        }
                    }
                    else if ( strcmp( "properties", layer_entry.name ) == 0 )
                    {
                        if ( layer_entry.value->type != json_array )
                        {
                            Log::sendError( "JSON for map “" + slug + "” is malformed: layer properties isn’t an array." );
                        }

                        for ( unsigned int k = 0; k < layer_entry.value->u.array.length; ++k )
                        {
                            const auto & prop = layer_entry.value->u.array.values[ k ];
                            if ( prop->type != json_object )
                            {
                                Log::sendError( "JSON for map “" + slug + "” is malformed: layer property isn’t an object." );
                            }

                            std::string name = "";
                            std::variant<bool, int, float, std::string> value = false;
                            
                            for ( unsigned int m = 0; m < prop->u.object.length; ++m )
                            {
                                const auto & prop_entry = prop->u.object.values[ m ];
                                if ( strcmp( "name", prop_entry.name ) == 0 )
                                {
                                    if ( prop_entry.value->type != json_string )
                                    {
                                        Log::sendError( "JSON for map “" + slug + "” is malformed: layer property name isn’t a string." );
                                    }

                                    name = std::string( prop_entry.value->u.string.ptr );
                                }
                                else if ( strcmp( "value", prop_entry.name ) == 0 )
                                {
                                    switch ( prop_entry.value->type )
                                    {
                                        case ( json_integer ):
                                        {
                                            value = int( prop_entry.value->u.integer );
                                        }
                                        break;
                                        case ( json_double ):
                                        {
                                            value = float( prop_entry.value->u.dbl );
                                        }
                                        break;
                                        case ( json_string ):
                                        {
                                            value = std::string( prop_entry.value->u.string.ptr );
                                        }
                                        break;
                                        case ( json_boolean ):
                                        {
                                            value = bool( prop_entry.value->u.boolean );
                                        }
                                        break;
                                        default:
                                        {
                                            // Do nothing.
                                        }
                                        break;
                                    }
                                }
                            }

                            if ( name != "" )
                            {
                                l.props.insert( { name, value } );
                            }
                        }
                    }
                }
                layers.push_back( l );
            }
        }
    }

    for ( const auto & layer : layers )
    {
        /*
        printf( "Type: %d\n", ( int )( layer.type ) );
        printf( "Width: %d\n", layer.width );
        printf( "Height: %d\n", layer.height );
        printf( "Tiles: " );
        for ( const auto tile : layer.tiles )
        {
            printf( "%d, ", tile );
        }
        printf( "\n" );
        auto it = layer.props.begin();
        while ( it != layer.props.end() )
        {
            std::cout << it->first << ": ";
            std::visit
            (
                []( const auto & s ) { std::cout << s << "\n"; },
                it->second
            );
            ++it;
        }
        printf( "\n" );*/

        if ( layer.width != width )
        {
            Log::sendError( "JSON for map “" + slug + "” is malformed: layer width != map width." );
        }
        else if ( layer.height != height )
        {
            Log::sendError( "JSON for map “" + slug + "” is malformed: layer height != map height." );
        }
    }

    json_value_free( root );
};

void Map::init( unsigned int state )
{
    for ( int i = 0; i < width * height; ++i ) {
        objs.push_back( {} );
    }

    for ( const auto & layer : layers )
    {
        switch ( layer.type )
        {
            case ( MapLayer::Type::TILES ):
            {
                Tile tiles[ width * height ];
                for ( int i = 0; i < width * height; ++i )
                {
                    const int n = layer.tiles[ i ] - 4097;
                    const int tileset_w = 16;
                    tiles[ i ].x = ( unsigned char )( n % tileset_w );
                    tiles[ i ].y = ( unsigned char )( std::floor( ( double )( n ) / ( double )( tileset_w ) ) );
                    tiles[ i ].palette = ( i % 2 ) + 1;
                    tiles[ i ].animation = ( n < 0 ) ? 255 : 0;
                }
                Render::addTilemap( "urban", tiles, width, height, state, Layer::BLOCKS_1 );
            }
            break;
            case ( MapLayer::Type::OBJ ):
            {
                Tile tiles[ width * height ];
                for ( int i = 0; i < width * height; ++i )
                {
                    const int n = layer.tiles[ i ] - 512;
                    tiles[ i ].x = ( unsigned char )( 0 );
                    tiles[ i ].y = ( unsigned char )( 0 );
                    tiles[ i ].palette = 1;
                    tiles[ i ].animation = ( n < 0 ) ? 255 : 6;
                }
                auto tilemap = Render::addTilemap( "objects", tiles, width, height, state, Layer::BLOCKS_1 );

                for ( int i = 0; i < width * height; ++i )
                {
                    const int n = layer.tiles[ i ] - 512;
                    if ( n >= 0 )
                    {
                        objs[ i ].push_back({ MapObjType::GEM, tilemap, { { "amount", 100 } } } );
                    }
                }
            }
            break;
            default:
            {
                // Do nothing.
            }
            break;
        }
    }
};
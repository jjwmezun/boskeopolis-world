#include <cmath>
#include "dir.hpp"
#include "json.hpp"
#include "game.hpp"
#include "layer.hpp"
#include "map.hpp"
#include "nasr.h"
#include "tileset.hpp"
#include "utility.hpp"

namespace BSW
{
    struct MapTileLayer
    {
        enum class Type
        {
            __NULL,
            COLLISION,
            TILE,
            OBJECT,
            SPRITE,
            IMAGE,
            WARP,
            GRADIENT,
            RAIN,
            CONSTELLATION,
            PALCHANGE
        };

        struct Image
        {
            char * texture;
            bool tilex;
            bool tiley;
        };

        struct Gradient
        {
            Dir::XY dir;
            unsigned int start;
            unsigned int end;
            uint_fast8_t palette;
        };

        struct Rain
        {
            unsigned int start;
            unsigned int end;
        };

        struct PalChange
        {
            unsigned int start;
            unsigned int end;
        };

        union Misc
        {
            Image image;
            Gradient gradient;
            Rain rain;
            PalChange palchange;
        };

        std::vector<int> tiles;
        MapTileLayer::Type type;
        float scrollx;
        float scrolly;
        float offsetx;
        float offsety;
        float tilex;
        Misc misc;
        Layer layer;
    };

    void Map::init( const Game & game )
    {
        std::string slug { "city-00" };
        JSON json { "assets/maps/" + slug + ".json" };
        width_ = json.getInt( "width" );
        height_ = json.getInt( "height" );
        JSONArray l = json.getArray( "layers" );

        unsigned int ii = 0;
        std::vector<Warp> warps;
        std::vector<unsigned int> object_layers;
        std::vector<MapTileLayer> layers = JSONMap<MapTileLayer>
        (
            l,
            [&]( const JSONItem & i )
            {
                MapTileLayer layer;
                layer.type = MapTileLayer::Type::__NULL;
                layer.scrollx = 0.0f;
                layer.scrolly = 0.0f;
                layer.offsetx = 0.0f;
                layer.offsety = 0.0f;
                JSONObject o = i.asObject();

                std::string type = o.getString( "type" );

                if ( type == "imagelayer" )
                {
                    layer.type = MapTileLayer::Type::IMAGE;
                    layer.misc.image.texture = nullptr;
                    layer.misc.image.tilex = false;
                    layer.misc.image.tiley = false;

                    std::string image = strReplace( o.getString( "image" ), "../graphics/", "" );
                    layer.misc.image.texture = static_cast<char *>( calloc( strlen( image.c_str() ) + 1, sizeof( char ) ) );
                    strcpy( layer.misc.image.texture, image.c_str() );

                    if ( o.hasBool( "repeatx" ) )
                    {
                        layer.misc.image.tilex = o.getBool( "repeatx" );
                    }

                    if ( o.hasBool( "repeaty" ) )
                    {
                        layer.misc.image.tiley = o.getBool( "repeaty" );
                    }

                    if ( o.hasFloat( "parallaxx" ) )
                    {
                        layer.scrollx = o.getFloat( "parallaxx" );
                    }
                    else if ( o.hasInt( "parallaxx" ) )
                    {
                        layer.scrollx = static_cast<float> ( o.getInt( "parallaxx" ) );
                    }

                    if ( o.hasFloat( "parallaxy" ) )
                    {
                        layer.scrolly = o.getFloat( "parallaxy" );
                    }
                    else if ( o.hasInt( "parallaxy" ) )
                    {
                        layer.scrolly = static_cast<float> ( o.getInt( "parallaxy" ) );
                    }

                    if ( o.hasFloat( "offsetx" ) )
                    {
                        layer.offsetx = o.getFloat( "offsetx" );
                    }
                    else if ( o.hasInt( "offsetx" ) )
                    {
                        layer.offsetx = static_cast<float> ( o.getInt( "offsetx" ) );
                    }

                    if ( o.hasFloat( "offsety" ) )
                    {
                        layer.offsety = o.getFloat( "offsety" );
                    }
                    else if ( o.hasInt( "offsety" ) )
                    {
                        layer.offsety = static_cast<float> ( o.getInt( "offsety" ) );
                    }
                }
                else if ( type == "objectgroup" )
                {
                    std::string name = o.getString( "name" );
                    layer.type = BSW::findInMap<MapTileLayer::Type>
                    (
                        {
                            { "warp", MapTileLayer::Type::WARP },
                            { "gradient", MapTileLayer::Type::GRADIENT },
                            { "rain", MapTileLayer::Type::RAIN },
                            { "constellation", MapTileLayer::Type::CONSTELLATION },
                            { "palchange", MapTileLayer::Type::PALCHANGE }
                        },
                        name,
                        "Map " + slug + " has invalid layer type " + name
                    );

                    switch ( layer.type )
                    {
                        case ( MapTileLayer::Type::WARP ):
                        {
                            if ( !o.hasArray( "objects" ) )
                            {
                                break;
                            }
                            const JSONArray objs = o.getArray( "objects" );
                            objs.forEach
                            (
                                [ & ]( const JSONItem & oi )
                                {
                                    Warp w;
                                    const auto oo = oi.asObject();
                                    if ( oo.hasFloat( "x" ) )
                                    {
                                        w.coords.x = oo.getFloat( "x" );
                                    }
                                    else if ( oo.hasInt( "x" ) )
                                    {
                                        w.coords.x = static_cast<float> ( oo.getInt( "x" ) );
                                    }
                                    if ( oo.hasFloat( "y" ) )
                                    {
                                        w.coords.y = oo.getFloat( "y" );
                                    }
                                    else if ( oo.hasInt( "y" ) )
                                    {
                                        w.coords.y = static_cast<float> ( oo.getInt( "y" ) );
                                    }
                                    if ( oo.hasFloat( "width" ) )
                                    {
                                        w.coords.w = oo.getFloat( "width" );
                                    }
                                    else if ( oo.hasInt( "width" ) )
                                    {
                                        w.coords.w = static_cast<float> ( oo.getInt( "width" ) );
                                    }
                                    if ( oo.hasFloat( "height" ) )
                                    {
                                        w.coords.h = oo.getFloat( "height" );
                                    }
                                    else if ( oo.hasInt( "height" ) )
                                    {
                                        w.coords.h = static_cast<float> ( oo.getInt( "height" ) );
                                    }
                                    if ( oo.hasArray( "properties" ) )
                                    {
                                        const JSONArray props = oo.getArray( "properties" );
                                        props.forEach
                                        (
                                            [ &w ]( const JSONItem & pi )
                                            {
                                                const auto po = pi.asObject();
                                                if ( !po.hasString( "name" ) )
                                                {
                                                    return;
                                                }
                                                const std::string name = po.getString( "name" );
                                                if ( name == "map" )
                                                {
                                                    if ( po.hasInt( "value" ) )
                                                    {
                                                        w.map = static_cast<unsigned int> ( po.getInt( "value" ) );
                                                    }
                                                    else if ( po.hasFloat( "value" ) )
                                                    {
                                                        w.map = static_cast<unsigned int> ( po.getFloat( "value" ) );
                                                    }
                                                }
                                                else if ( name == "entrance_x" )
                                                {
                                                    if ( po.hasFloat( "value" ) )
                                                    {
                                                        w.entrance_x = static_cast<float>( blocksToPixels( po.getFloat( "value" ) ) );
                                                    }
                                                    else if ( po.hasInt( "value" ) )
                                                    {
                                                        w.entrance_x = static_cast<float>( blocksToPixels( po.getInt( "value" ) ) );
                                                    }
                                                }
                                                else if ( name == "entrance_y" )
                                                {
                                                    if ( po.hasFloat( "value" ) )
                                                    {
                                                        w.entrance_y = static_cast<float>( blocksToPixels( po.getFloat( "value" ) ) );
                                                    }
                                                    else if ( po.hasInt( "value" ) )
                                                    {
                                                        w.entrance_y = static_cast<float>( blocksToPixels( po.getInt( "value" ) ) );
                                                    }
                                                }
                                                else if ( name == "camera_x" )
                                                {
                                                    if ( po.hasFloat( "value" ) )
                                                    {
                                                        w.camera_x = po.getFloat( "value" );
                                                    }
                                                    else if ( po.hasInt( "value" ) )
                                                    {
                                                        w.camera_x = static_cast<float> ( po.getInt( "value" ) );
                                                    }
                                                }
                                                else if ( name == "camera_y" )
                                                {
                                                    if ( po.hasFloat( "value" ) )
                                                    {
                                                        w.camera_y = po.getFloat( "value" );
                                                    }
                                                    else if ( po.hasInt( "value" ) )
                                                    {
                                                        w.camera_y = static_cast<float> ( po.getInt( "value" ) );
                                                    }
                                                }
                                            }
                                        );
                                    }
                                    warps.push_back( w );
                                }
                            );
                        }
                        break;
                        case ( MapTileLayer::Type::GRADIENT ):
                        {
                            layer.misc.gradient.dir = Dir::XY::UP;
                            layer.misc.gradient.start = 0;
                            layer.misc.gradient.end = 0;
                            layer.misc.gradient.palette = 0;

                            if ( o.hasArray( "properties" ) )
                            {
                                const JSONArray props = o.getArray( "properties" );
                                props.forEach
                                (
                                    [ &layer ]( const JSONItem & di )
                                    {
                                        const JSONObject io = di.asObject();
                                        const std::string name = io.getString( "name" );
                                        if ( name == "direction" )
                                        {
                                            std::string dir = io.getString( "value" );
                                            layer.misc.gradient.dir = Dir::getXYFromString( dir );
                                        }
                                        else if ( name == "start" )
                                        {
                                            if ( io.hasFloat( "value" ) )
                                            {
                                                layer.misc.gradient.start = static_cast<unsigned int> ( io.getFloat( "value" ) );
                                            }
                                            else
                                            {
                                                layer.misc.gradient.start = static_cast<unsigned int> ( io.getInt( "value" ) );
                                            }
                                        }
                                        else if ( name == "end" )
                                        {
                                            if ( io.hasFloat( "value" ) )
                                            {
                                                layer.misc.gradient.end = static_cast<unsigned int> ( io.getFloat( "value" ) );
                                            }
                                            else
                                            {
                                                layer.misc.gradient.end = static_cast<unsigned int> ( io.getInt( "value" ) );
                                            }
                                        }
                                        else if ( name == "palette" )
                                        {
                                            if ( io.hasFloat( "value" ) )
                                            {
                                                layer.misc.gradient.palette = static_cast<uint_fast8_t> ( io.getFloat( "value" ) );
                                            }
                                            else
                                            {
                                                layer.misc.gradient.palette = static_cast<uint_fast8_t> ( io.getInt( "value" ) );
                                            }
                                        }
                                    }
                                );
                            }

                        }
                        break;
                        case ( MapTileLayer::Type::RAIN ):
                        {
                            layer.misc.rain.start = 0;
                            layer.misc.rain.end = 0;

                            if ( o.hasArray( "properties" ) )
                            {
                                const JSONArray props = o.getArray( "properties" );
                                props.forEach
                                (
                                    [ &layer ]( const JSONItem & di )
                                    {
                                        const JSONObject io = di.asObject();
                                        const std::string name = io.getString( "name" );
                                        if ( name == "start" )
                                        {
                                            if ( io.hasFloat( "value" ) )
                                            {
                                                layer.misc.rain.start = static_cast<unsigned int> ( io.getFloat( "value" ) );
                                            }
                                            else
                                            {
                                                layer.misc.rain.start = static_cast<unsigned int> ( io.getInt( "value" ) );
                                            }
                                        }
                                        else if ( name == "end" )
                                        {
                                            if ( io.hasFloat( "value" ) )
                                            {
                                                layer.misc.rain.end = static_cast<unsigned int> ( io.getFloat( "value" ) );
                                            }
                                            else
                                            {
                                                layer.misc.rain.end = static_cast<unsigned int> ( io.getInt( "value" ) );
                                            }
                                        }
                                    }
                                );
                            }

                        }
                        break;
                        case ( MapTileLayer::Type::CONSTELLATION ):
                        {
                            layer.scrollx = 0.0f;
                            layer.scrolly = 0.0f;

                            if ( o.hasArray( "properties" ) )
                            {
                                const JSONArray props = o.getArray( "properties" );
                                props.forEach
                                (
                                    [ &layer ]( const JSONItem & di )
                                    {
                                        const JSONObject io = di.asObject();
                                        const std::string name = io.getString( "name" );
                                        if ( name == "scrollx" )
                                        {
                                            if ( io.hasInt( "value" ) )
                                            {
                                                layer.scrollx = static_cast<float> ( io.getInt( "value" ) );
                                            }
                                            else
                                            {
                                                layer.scrollx = io.getFloat( "value" );
                                            }
                                        }
                                        else if ( name == "scrolly" )
                                        {
                                            if ( io.hasInt( "value" ) )
                                            {
                                                layer.scrolly = static_cast<float> ( io.getInt( "value" ) );
                                            }
                                            else
                                            {
                                                layer.scrolly = io.getFloat( "value" );
                                            }
                                        }
                                    }
                                );
                            }

                        }
                        break;
                        case ( MapTileLayer::Type::PALCHANGE ):
                        {
                            layer.misc.palchange.start = 0;
                            layer.misc.palchange.end = 0;

                            if ( o.hasArray( "properties" ) )
                            {
                                const JSONArray props = o.getArray( "properties" );
                                props.forEach
                                (
                                    [ &layer ]( const JSONItem & di )
                                    {
                                        const JSONObject io = di.asObject();
                                        const std::string name = io.getString( "name" );
                                        if ( name == "start" )
                                        {
                                            if ( io.hasFloat( "value" ) )
                                            {
                                                layer.misc.palchange.start = static_cast<unsigned int> ( io.getFloat( "value" ) );
                                            }
                                            else
                                            {
                                                layer.misc.palchange.start = static_cast<unsigned int> ( io.getInt( "value" ) );
                                            }
                                        }
                                        else if ( name == "end" )
                                        {
                                            if ( io.hasFloat( "value" ) )
                                            {
                                                layer.misc.palchange.end = static_cast<unsigned int> ( io.getFloat( "value" ) );
                                            }
                                            else
                                            {
                                                layer.misc.palchange.end = static_cast<unsigned int> ( io.getInt( "value" ) );
                                            }
                                        }
                                    }
                                );
                            }

                        }
                        break;
                    }
                }
                else if ( type == "tilelayer" )
                {
                    JSONArray tiles = o.getArray( "data" );

                    std::string name = o.getString( "name" );
                    const MapTileLayer::Type type = BSW::findInMap<MapTileLayer::Type>
                    (
                        {
                            { "collision", MapTileLayer::Type::COLLISION },
                            { "tile", MapTileLayer::Type::TILE },
                            { "object", MapTileLayer::Type::OBJECT },
                            { "sprite", MapTileLayer::Type::SPRITE }
                        },
                        name,
                        "Map " + slug + " has invalid layer type " + name
                    );

                    float scrollx = 0.0f;
                    float scrolly = 0.0f;
                    unsigned int tilex = 0;
                    Layer lnum = Layer::BLOCKS_1;

                    if ( o.hasArray( "properties" ) )
                    {
                        const JSONArray props = o.getArray( "properties" );
                        props.forEach
                        (
                            [ &scrollx, &scrolly, &tilex, &lnum ]( const JSONItem & di )
                            {
                                const JSONObject io = di.asObject();
                                const std::string name = io.getString( "name" );
                                if ( name == "scrollx" )
                                {
                                    if ( io.hasInt( "value" ) )
                                    {
                                        scrollx = static_cast<float> ( io.getInt( "value" ) );
                                    }
                                    else
                                    {
                                        scrollx = io.getFloat( "value" );
                                    }
                                }
                                else if ( name == "scrolly" )
                                {
                                    if ( io.hasInt( "value" ) )
                                    {
                                        scrolly = static_cast<float> ( io.getInt( "value" ) );
                                    }
                                    else
                                    {
                                        scrolly = io.getFloat( "value" );
                                    }
                                }
                                else if ( name == "tilex" )
                                {
                                    tilex = io.getInt( "value" );
                                }
                                else if ( name == "layer" )
                                {
                                    if ( io.hasString( "value" ) )
                                    {
                                        const std::string lval = io.getString( "value" );
                                        lnum = getLayerFromString( lval ).value_or( lnum );
                                    }
                                    else if ( io.hasInt( "value" ) )
                                    {
                                        lnum = static_cast<Layer> ( io.getInt( "value" ) );
                                    }
                                }
                            }
                        );
                    }

                    layer =
                    {
                        JSONMap<int>
                        (
                            tiles,
                            []( const JSONItem & di )
                            {
                                return di.asInt();
                            }
                        ),
                        type,
                        scrollx,
                        scrolly,
                        0.0f,
                        0.0f,
                        static_cast<float>( tilex ),
                        {},
                        lnum
                    };

                    if ( type == MapTileLayer::Type::OBJECT )
                    {
                        object_layers.push_back( ii );
                    }
                }

                ++ii;
                return layer;
            }
        );

        warpcount_ = warps.size();
        warps_ = static_cast<Warp *>( calloc( warpcount_, sizeof( Warp ) ) );
        for ( unsigned int i = 0; i < warpcount_; ++i )
        {
            warps_[ i ] = warps[ i ];
        }

        collision_ = static_cast<CollisionNode *> ( calloc( width_ * height_, sizeof( CollisionNode ) ) );

        std::unordered_map<std::string, Tileset> tilesets;
        for ( unsigned int i = 0; i < layers.size(); ++i )
        {
            const MapTileLayer & layer = layers[ i ];
            switch ( layer.type )
            {
                case ( MapTileLayer::Type::COLLISION ):
                {
                    for ( unsigned int i = 0; i < layer.tiles.size(); ++i )
                    {
                        if ( layer.tiles[ i ] > 0 )
                        {
                            if ( collision_[ i ].type == CollisionType::NONE )
                            {
                                collision_[ i ].type = static_cast<CollisionType> ( layer.tiles[ i ] );
                            }
                            else
                            {
                                CollisionNode * node = &collision_[ i ];
                                while ( node->next )
                                {
                                    node = node->next;
                                }
                                node->next = static_cast<CollisionNode *> ( calloc( 1, sizeof( CollisionNode ) ) );
                                node->next->type = static_cast<CollisionType> ( layer.tiles[ i ] );
                            }
                        }
                    }
                }
                break;
                case ( MapTileLayer::Type::TILE ):
                {
                    unsigned int w = layer.tilex == 0 ? width_ : layer.tilex;
                    unsigned int wx = blocksToPixels( w );
                    float tilingx = layer.tilex == 0 ? 1.0f : std::ceil( static_cast<float> ( getWidthPixels() ) / static_cast<float> ( wx ) );
                    float fullw = tilingx * wx;
                    int texture = NasrAddTextureBlankEx( wx, getHeightPixels(), NASR_SAMPLING_DEFAULT, NASR_INDEXED_NO );
                    int menu_texture = NasrLoadFileAsTexture( "assets/graphics/tilesets/urban.png" );
                    if ( menu_texture < 0 )
                    {
                        // TODO: Throw exception.
                    }
                    NasrRect src = { 0.0f, 0.0f, 16.0f, 16.0f };
                    NasrRect dest = src;
                    NasrSetTextureAsTarget( texture );

                    std::string name { "urban" };
                    auto it = tilesets.find( name );
                    if ( it == tilesets.end() )
                    {
                        Tileset t { name };
                        t.init();
                        tilesets.insert( std::pair<std::string, Tileset> ( name, t ) );
                        it = tilesets.find( name );
                    }
                    Tileset & tileset = it->second;
                    bool has_dynamic_tiles = false;
                    std::vector<NasrTile> tiles;
                    
                    for ( int y = 0; y < height_; ++y )
                    {
                        for ( int x = 0; x < w; ++x )
                        {
                            unsigned int i = y * width_ + x % width_;
                            const int tile = layer.tiles[ i ];
                            NasrTile nasrtile { 0, 0, 0, 255 };
                            if ( tile > 0 )
                            {
                                bool is_dynamic_tile = false;
                                const int ti = tile - 7073;
                                const unsigned char src_x = ti % 16;
                                const unsigned char src_y = static_cast<unsigned char> ( ti / 16.0f );
                                unsigned char animation = 0;
                                unsigned char palette = 0;
                                src.x = static_cast<float> ( src_x ) * 16.0f;
                                src.y = std::floor( static_cast<float> ( src_y ) * 16.0f );
                                dest.x = static_cast<float> ( i % width_ ) * 16.0f;
                                dest.y = std::floor( i / width_ ) * 16.0f;

                                try
                                {
                                    BlockType bt = tileset.getBlockType( ti );
                                    palette = bt.getPalette();
                                    animation = bt.getAnimation();
                                    if ( animation > 1 )
                                    {
                                        has_dynamic_tiles = true;
                                        is_dynamic_tile = true;
                                    }
                                }
                                catch ( std::runtime_error & error )
                                {

                                }

                                if ( is_dynamic_tile )
                                {
                                    nasrtile = { src_x, src_y, palette, animation };
                                }
                                else
                                {
                                    NasrDrawSpriteToTexture
                                    (
                                        menu_texture,
                                        src,
                                        dest,
                                        0,
                                        0,
                                        0.0f,
                                        0.0f,
                                        0.0f,
                                        1.0f,
                                        palette,
                                        0,
                                        1.0f,
                                        1.0f
                                    ); 
                                }

                            }

                            tiles.push_back( nasrtile );
                        }
                    }

                    NasrReleaseTextureTarget();

                    game.render().addTextureSprite
                    (
                        texture,
                        0.0f,
                        0.0f,
                        static_cast<float> ( fullw ),
                        static_cast<float> ( getHeightPixels() ),
                        0.0f,
                        0.0f,
                        { { "scrollx", layer.scrollx }, { "scrolly", layer.scrolly }, { "layer", layer.layer }, { "tilingx", tilingx }, { "srcw", static_cast<float>( wx ) } }
                    );

                    if ( has_dynamic_tiles )
                    {
                        game.render().addTilemap
                        (
                            "urban",
                            tiles,
                            w,
                            height_,
                        { { "scrollx", layer.scrollx }, { "scrolly", layer.scrolly }, { "layer", layer.layer }, { "tilingx", tilingx } }
                        );
                    }
                }
                break;
                case ( MapTileLayer::Type::IMAGE ):
                {
                    const unsigned int texture_id = game.render().getTextureId( layer.misc.image.texture );
                    const float width = static_cast<float> ( NasrTextureGetWidth( texture_id ) );
                    const float height = static_cast<float> ( NasrTextureGetHeight( texture_id ) );
                    const float bgtilex = layer.misc.image.tilex ? std::ceil( static_cast<float> ( getWidthPixels() ) / width ) : 1.0f;
                    const float bgtotalw = bgtilex * width;
                    const float bgtiley = layer.misc.image.tiley ? std::ceil( static_cast<float> ( getHeightPixels() ) / height ) : 1.0f;
                    const float bgtotalh = bgtiley * height;
                    game.render().addSprite
                    (
                        layer.misc.image.texture,
                        0.0f,
                        0.0f,
                        bgtotalw,
                        bgtotalh,
                        layer.offsetx,
                        layer.offsety,
                        {
                            { "layer", Layer::BG_1 },
                            { "scrolly", layer.scrolly },
                            { "scrollx", layer.scrollx },
                            { "tilingx", bgtilex },
                            { "tilingx", bgtiley },
                            { "srcw", width }
                        }
                    );
                }
                break;
                case ( MapTileLayer::Type::GRADIENT ):
                {
                    game.render().addRectGradient
                    (
                        0.0f,
                        0.0f,
                        BSW::WINDOW_WIDTH_PIXELS,
                        BSW::WINDOW_HEIGHT_PIXELS,
                        layer.misc.gradient.dir,
                        layer.misc.gradient.start,
                        layer.misc.gradient.end,
                        { { "scrollx", 1.0f }, { "scrolly", 1.0f }, { "palette", layer.misc.gradient.palette } }
                    );
                }
                break;
            }
        }
    };

    void Map::close()
    {
        free( warps_ );
        for ( unsigned int i = 0; i < width_ * height_; ++i )
        {
            CollisionNode * next = collision_[ i ].next;
            while ( next )
            {
                CollisionNode * tempnext = next->next;
                free( next );
                next = tempnext;
            }
        }
        free( collision_ );
    };

    bool Map::testCollision( unsigned int x, unsigned int y, CollisionType type ) const
    {
        if ( x > width_ || y > height_ ) return false;

        const int i = getIFromXAndY( x, y );

        for ( CollisionNode * node = &collision_[ i ]; node; node = node->next )
        {
            if ( node->type == type )
            {
                return true;
            }
        }
        return false;
    };
}
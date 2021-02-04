#include "asset.h"
#include "assert.h"
#include "bswmath.h"
#include "camera.h"
#include "color.h"
#include "config.h"
#include "graphics.h"
#include "json.h"
#include "map.h"
#include "rect.h"
#include "render.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char * layer_types[ NUM_O_LAYER_TYPES ] = {
    "bg1t",
    "bg2t",
    "bg3t",
    "fg1t",
    "fg2t",
    "fg3t"
};

typedef struct map_layer_t
{
    char * type;
    int_fast16_t * tiles;
    int num_o_tiles;
} map_layer_t;

static int map_test_generic_collision( const struct map_t * map, int x, int y, int_fast16_t index );

void map_update( const struct map_t * map, const struct camera_t * camera )
{
    for ( int i = 0; i < NUM_O_LAYER_TYPES; ++i )
    {
        if ( map->bg_graphics_ids[ i ] > 0 )
        {
            // Make graphics relative to camera.
            graphics_data_regular_t * graphics = &render_get_graphics( map->bg_graphics_ids[ i ] )->data.regular;
            graphics->dest.x = -camera->position.x;
            graphics->dest.y = -camera->position.y;
        }
    }
};

int map_index( const struct map_t * map, int x, int y )
{
    return XYTOI( map->width, x, y );
};

int map_test_pixel_solid_collision( const struct map_t * map, int x, int y )
{
    return map_test_generic_collision( map, x, y, 1 );
};

int map_test_pixel_top_solid_collision( const struct map_t * map, int x, int y )
{
    return map_test_generic_collision( map, x, y, 2 );
};

int map_test_pixel_ladder_collision( const struct map_t * map, int x, int y )
{
    return map_test_generic_collision( map, x, y, 3 );
};

int map_test_pixel_gem_collision( const struct map_t * map, int x, int y )
{
    return map_test_generic_collision( map, x, y, 4 );
};

int map_test_pixel_treasure_collision( const struct map_t * map, int x, int y )
{
    return map_test_generic_collision( map, x, y, 5 );
};

void map_remove( struct map_t * map, int x, int y )
{
    map->collision[ map_index( map, x, y ) ] = 0;
};

map_t map_create()
{
    map_t map;

    char * filename = asset_map( "blueberry-1.json" );
    char * text = asset_read_file_text( filename );
    free( filename );
    if ( !text )
    {
        printf( "%s\n", "Couldn’t read map json file." );
        return map;
    }

    json_char * json = ( json_char * )( text );
    json_value * root = json_parse( json, strlen( text ) );
    free( text );

    if ( !root )
    {
        printf( "%s\n", "Error parsing map json file." );
        return map;
    }

    int num_o_layers = 0;
    map_layer_t layers[ 10 ];

    if ( root->type != json_object )
    {
        json_value_free( root );
        printf( "%s\n", "JSON PARSE ERROR." );
    }

    for ( int i = 0; i < root->u.object.length; ++i )
    {
        const json_char * name = root->u.object.values[ i ].name;
        if ( strcmp( name, "width" ) == 0 )
        {
            json_value * value = root->u.object.values[ i ].value;
            if ( value->type != json_integer )
            {
                printf( "%s\n", "JSON PARSE ERROR." );
            }
            map.width = value->u.integer;
        }
        else if ( strcmp( name, "height" ) == 0 )
        {
            json_value * value = root->u.object.values[ i ].value;
            if ( value->type != json_integer )
            {
                printf( "%s\n", "JSON PARSE ERROR." );
            }
            map.height = value->u.integer;
        }
        else if ( strcmp( name, "layers" ) == 0 )
        {
            const json_value * jlayers = root->u.object.values[ i ].value;
            if ( jlayers->type != json_array )
            {
                printf( "%s\n", "Error parsing map json file: layers is not array." );
            }
            for ( int li = 0; li < jlayers->u.array.length; ++li )
            {
                json_value * layer = jlayers->u.array.values[ li ];
                if ( layer->type != json_object )
                {
                    printf( "%s\n", "Error parsing map json file: layer is not object." );
                }
                for ( int ki = 0; ki < layer->u.object.length; ++ki )
                {
                    const json_char * name = layer->u.object.values[ ki ].name;
                    if ( strcmp( name, "name" ) == 0 )
                    {
                        const json_value * type = layer->u.object.values[ ki ].value;
                        if ( type->type != json_string )
                        {
                            printf( "%s\n", "Error parsing map json file: layer type is not string." );
                        }
                        layers[ num_o_layers ].type = type->u.string.ptr;
                    }
                    else if ( strcmp( name, "data" ) == 0 )
                    {
                        const json_value * data = layer->u.object.values[ ki ].value;
                        if ( data->type != json_array )
                        {
                            printf( "%s\n", "Error parsing map json file: layer data is not array." );
                        }
                        else
                        {
                            layers[ num_o_layers ].num_o_tiles = data->u.array.length;
                            layers[ num_o_layers ].tiles = calloc( data->u.array.length, sizeof( int_fast16_t ) );
                            if ( !layers[ num_o_layers ].tiles )
                            {
                                printf( "%s\n", "Failed to allocate map." );
                            }
                            for ( int di = 0; di < data->u.array.length; ++di )
                            {
                                const json_value * tile = data->u.array.values[ di ];
                                if ( tile->type != json_integer )
                                {
                                    printf( "%s\n", "Error parsing map json file: layer data item is not integer." );
                                }
                                layers[ num_o_layers ].tiles[ di ] = tile->u.integer;
                            }
                        }
                    }
                }
                ++num_o_layers;
            }
        }
    }

    map.collision = calloc( map.width * map.height, sizeof( int_fast16_t ) );
    int_fast16_t gfx_layers[ NUM_O_LAYER_TYPES ][ 3 ][ map.width * map.height ];
    int gfx_layers_nums[ NUM_O_LAYER_TYPES ] = { 0, 0, 0, 0, 0, 0 };
    for ( int i = 0; i < num_o_layers; ++i )
    {
        if ( strcmp( layers[ i ].type, "collision" ) == 0 )
        {
            for ( int tile = 0; tile < layers[ i ].num_o_tiles && i < map.width * map.height; ++tile )
            {
                if ( layers[ i ].tiles[ tile ] )
                {
                    map.collision[ tile ] = layers[ i ].tiles[ tile ];
                }
            }
            continue;
        }

        for ( int lt = 0; lt < NUM_O_LAYER_TYPES; ++lt )
        {
            if ( strcmp( layers[ i ].type, layer_types[ lt ] ) == 0 && gfx_layers_nums[ lt ] < 2 )
            {
                memcpy( gfx_layers[ lt ][ gfx_layers_nums[ lt ] ], layers[ i ].tiles, sizeof( int_fast16_t ) * map.width * map.height );
                ++gfx_layers_nums[ lt ];
                continue;
            }
        }
    }


    char texture_name[ 5 ] = "MBG1";
    for ( int lti = 0; lti < NUM_O_LAYER_TYPES; ++lti )
    {
        texture_name[ 3 ] = ( char )( lti );
        const int bg_texture_id = render_create_custom_texture( texture_name, BLOCKS_TO_PIXELS( map.width ), BLOCKS_TO_PIXELS( map.height ) );
        if ( bg_texture_id != -1 )
        {
            layer_t layer = LAYER_BG1;
            switch ( lti )
            {
                case ( 1 ): layer = LAYER_BG2; break;
                case ( 2 ): layer = LAYER_BG3; break;
                case ( 3 ): layer = LAYER_FG1; break;
                case ( 4 ): layer = LAYER_FG2; break;
                case ( 5 ): layer = LAYER_FG3; break;
            }
            graphics_data_regular_t graphics_box = { -1, { 0, 0, BLOCK_SIZE, BLOCK_SIZE }, { 0, 0, BLOCK_SIZE, BLOCK_SIZE }, FLIP_NONE, 0.0 };
            render_set_target_texture( bg_texture_id );
            render_clear();
            graphics_t graphics = { GRAPHICS_REGULAR, layer, {{ bg_texture_id, { 0, 0, BLOCKS_TO_PIXELS( map.width ), BLOCKS_TO_PIXELS( map.height ) }, { 0, 0, BLOCKS_TO_PIXELS( map.width ), BLOCKS_TO_PIXELS( map.height ) }, FLIP_NONE, 0.0 }}};
            map.bg_graphics_ids[ lti ] = render_add_graphics( &graphics );
            graphics_box.texture = render_get_texture_id( "tilesets/urban.png" );
            for ( int li = 0; li < gfx_layers_nums[ lti ]; ++li )
            {
                for ( int tile = 0; tile < map.width * map.height; ++tile )
                {
                    const int tile_type = gfx_layers[ lti ][ li ][ tile ] - 6;
                    graphics_box.dest.x = BLOCKS_TO_PIXELS( ITOX( map.width, tile ) );
                    graphics_box.dest.y = BLOCKS_TO_PIXELS( ITOY( map.width, tile ) );
                    graphics_box.src.x = BLOCKS_TO_PIXELS( ITOX( 16, tile_type ) );
                    graphics_box.src.y = BLOCKS_TO_PIXELS( ITOY( 16, tile_type ) );
                    render_sprite( &graphics_box );
                }
            }
            render_release_target_texture();
        }
    }

    free_extra_map:
        json_value_free( root );
        for ( int i = 0; i < num_o_layers; ++i )
        {
            free( layers[ i ].tiles );
        }
        return map;
};

void map_destroy( struct map_t * map )
{
    if ( map->collision )
    {
        free( map->collision );
    }
};

static int map_test_generic_collision( const struct map_t * map, int x, int y, int_fast16_t index )
{
    return
        x >= 0 &&
        x < map->width &&
        y >= 0 &&
        y < map->height &&
        map->collision[ map_index( map, x, y ) ] == index;
};
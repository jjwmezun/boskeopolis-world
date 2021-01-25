#include "asset.h"
#include "bswmath.h"
#include "camera.h"
#include "color.h"
#include "config.h"
#include "graphics.h"
#include "map.h"
#include "rect.h"
#include "render.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum map_create_state_t
{
    MCS_TYPE,
    MCS_WIDTH,
    MCS_HEIGHT,
    MCS_TILES
} map_create_state_t;

typedef enum map_create_type_t
{
    MCT_COLLISION,
    MCT_BG
} map_create_type_t;

static int map_test_generic_collision( const struct map_t * map, int x, int y, int index );

void map_update( const struct map_t * map, const struct camera_t * camera )
{
    if ( map->bg_graphics_id != -1 )
    {
        // Make graphics relative to camera.
        graphics_data_regular_t * graphics = &render_get_graphics( map->bg_graphics_id )->data.regular;
        graphics->dest.x = -camera->position.x;
        graphics->dest.y = -camera->position.y;
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
    map->tiles[ map_index( map, x, y ) ] = 0;
};

map_t map_create()
{
    map_t map;
    map_create_state_t state = MCS_TYPE;
    map_create_type_t type;
    char * filename = asset_map( "blueberry-1.bswm" );
    FILE * file = fopen ( filename, "rb" );
    int16_t current;
    int i;
    int w;
    int h;
    map.bg_graphics_id = -1;
    graphics_data_regular_t graphics_box = { -1, { 0, 0, BLOCK_SIZE, BLOCK_SIZE }, { 0, 0, BLOCK_SIZE, BLOCK_SIZE }, FLIP_NONE, 0.0 };

    // Loop over map file, 2 byte # @ a time.
    //
    // The format for the file is:
    // 1st bytes: type
    // 2nd bytes: width
    // 3rd bytes: height
    // 4th to ( w * h + 4 )th bytes: tiles
    //
    // After final tile bytes, it loops back to checking type.
    while ( fread( &current, sizeof( int16_t ), 1, file ) == 1 )
    {
        switch ( state )
        {
            case ( MCS_TYPE ):
            {
                if ( current == 0 )
                {
                    type = MCT_COLLISION;
                }
                else if ( current == 1 )
                {
                    type = MCT_BG;
                }
                state = MCS_WIDTH;
            }
            break;
            case ( MCS_WIDTH ):
            {
                w = ( int )( current );
                if ( type == MCT_COLLISION )
                {
                    map.width = w;
                }
                state = MCS_HEIGHT;
            }
            break;
            case ( MCS_HEIGHT ):
            {
                h = ( int )( current );
                if ( type == MCT_COLLISION )
                {
                    map.height = h;
                }
                state = MCS_TILES;
                i = 0;
            }
            break;
            case ( MCS_TILES ):
            {
                if ( i == w * h - 1 )
                {
                    state = MCS_TYPE;
                }
                if ( type == MCT_COLLISION )
                {
                    map.tiles[ i ] = ( int )( current );
                }
                else if ( type == MCT_BG )
                {
                    if ( map.bg_graphics_id == -1 )
                    {
                        const int bg_texture_id = render_create_custom_texture( "MBG1", BLOCKS_TO_PIXELS( w ), BLOCKS_TO_PIXELS( h ) );
                        if ( bg_texture_id != -1 )
                        {
                            render_set_target_texture( bg_texture_id );
                            graphics_t graphics = { GRAPHICS_REGULAR, {{ bg_texture_id, { 0, 0, BLOCKS_TO_PIXELS( w ), BLOCKS_TO_PIXELS( h ) }, { 0, 0, BLOCKS_TO_PIXELS( w ), BLOCKS_TO_PIXELS( h ) }, FLIP_NONE, 0.0 }}};
                            map.bg_graphics_id = render_add_graphics( &graphics );
                            graphics_box.texture = render_get_texture_id( "tilesets/urban.png" );
                        }
                    }
                    if ( current != 0 )
                    {
                        const int tile = ( int )( current ) - 6;
                        graphics_box.dest.x = BLOCKS_TO_PIXELS( ITOX( w, i ) );
                        graphics_box.dest.y = BLOCKS_TO_PIXELS( ITOY( w, i ) );
                        graphics_box.src.x = BLOCKS_TO_PIXELS( ITOX( 16, tile ) );
                        graphics_box.src.y = BLOCKS_TO_PIXELS( ITOY( 16, tile ) );
                        render_sprite( &graphics_box );
                    }
                }
                ++i;
            }
            break;
        }
    }
    if ( map.bg_graphics_id >= 0 )
    {
        render_release_target_texture();
    }
    free( filename );
    return map;
};

static int map_test_generic_collision( const struct map_t * map, int x, int y, int index )
{
    return
        x >= 0 &&
        x < map->width &&
        y >= 0 &&
        y < map->height &&
        map->tiles[ map_index( map, x, y ) ] == index;
};
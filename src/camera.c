#include "camera.h"
#include "config.h"
#include "map.h"
#include "sprite.h"

rect_t camera_relative( const camera_t * camera, rect_t coords )
{
    coords.x -= ( int )( camera->position.x );
    coords.y -= ( int )( camera->position.y );
    return coords;
};

double camera_bottom( const camera_t * camera )
{
    return camera->position.y + camera->position.h;
};

double camera_right( const camera_t * camera )
{
    return camera->position.x + camera->position.w;
};

void camera_update( camera_t * camera, const struct sprite_t * sprite, const int * map )
{
    // If sprite is past boundaries, move camera so that sprite is just within
    // boundaries.
    const double sprite_r = sprite_right( sprite );
    const double camera_right_boundary_size = camera->position.w * 0.75;
    if ( sprite_r > camera->position.x + camera_right_boundary_size )
    {
        camera->position.x = sprite_r - camera_right_boundary_size;
    }
    const double camera_left_boundary_size = camera->position.w * 0.25;
    if ( sprite->position.x < camera->position.x + camera_left_boundary_size )
    {
        camera->position.x = sprite->position.x - camera_left_boundary_size;
    }

    const double sprite_b = sprite_bottom( sprite );
    const double camera_bottom_boundary_size = camera->position.h * 0.666;
    if ( sprite_b > camera->position.y + camera_bottom_boundary_size )
    {
        camera->position.y = sprite_b - camera_bottom_boundary_size;
    }
    const double camera_top_boundary_size = camera->position.h * 0.2;
    if ( sprite->position.y < camera->position.y + camera_top_boundary_size )
    {
        camera->position.y = sprite->position.y - camera_top_boundary_size;
    }

    // Keep camera within map boundaries.
    const int map_width = BLOCKS_TO_PIXELS( map[ MAP_WIDTH_LOCATION ] );
    const int map_height = BLOCKS_TO_PIXELS( map[ MAP_HEIGHT_LOCATION ] );
    if ( camera_right( camera ) > map_width )
    {
        camera->position.x = map_width - camera->position.w;
    }
    else if ( camera->position.x < 0 )
    {
        camera->position.x = 0;
    }

    if ( camera_bottom( camera ) > map_height )
    {
        camera->position.y = map_height - camera->position.h;
    }
    else if ( camera->position.y < 0 )
    {
        camera->position.y = 0;
    }
};
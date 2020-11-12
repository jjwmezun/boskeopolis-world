#include "camera.h"
#include "config.h"
#include "map.h"
#include "sprite.h"

rect_t camera_relative( const camera_t * camera, rect_t coords )
{
    coords.x -= ( int )( camera->x );
    coords.y -= ( int )( camera->y );
    return coords;
};

double camera_bottom( const camera_t * camera )
{
    return camera->y + camera->h;
};

double camera_right( const camera_t * camera )
{
    return camera->x + camera->w;
};

void camera_update( camera_t * camera, const struct sprite_t * sprite, const struct map_t * map )
{
    // If sprite is past boundaries, move camera so that sprite is just within
    // boundaries.
    const double sprite_r = sprite_right( sprite );
    const double camera_right_boundary_size = camera->w * 0.75;
    if ( sprite_r > camera->x + camera_right_boundary_size )
    {
        camera->x = sprite_r - camera_right_boundary_size;
    }
    const double camera_left_boundary_size = camera->w * 0.25;
    if ( sprite->position.x < camera->x + camera_left_boundary_size )
    {
        camera->x = sprite->position.x - camera_left_boundary_size;
    }

    const double sprite_b = sprite_bottom( sprite );
    const double camera_bottom_boundary_size = camera->h * 0.666;
    if ( sprite_b > camera->y + camera_bottom_boundary_size )
    {
        camera->y = sprite_b - camera_bottom_boundary_size;
    }
    const double camera_top_boundary_size = camera->h * 0.2;
    if ( sprite->position.y < camera->y + camera_top_boundary_size )
    {
        camera->y = sprite->position.y - camera_top_boundary_size;
    }

    // Keep camera within map boundaries.
    const int map_width = BLOCKS_TO_PIXELS( map->width );
    const int map_height = BLOCKS_TO_PIXELS( map->height );
    if ( camera_right( camera ) > map_width )
    {
        camera->x = map_width - camera->w;
    }
    else if ( camera->x < 0 )
    {
        camera->x = 0;
    }

    if ( camera_bottom( camera ) > map_height )
    {
        camera->y = map_height - camera->h;
    }
    else if ( camera->y < 0 )
    {
        camera->y = 0;
    }
};
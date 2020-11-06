#include "camera.h"
#include "sprite.h"

rect_t camera_relative( const camera_t* camera, rect_t coords )
{
    coords.x -= ( int )( camera->x );
    coords.y -= ( int )( camera->y );
    return coords;
};

double camera_bottom( const camera_t* camera )
{
    return camera->y + camera->h;
};

double camera_right( const camera_t* camera )
{
    return camera->x + camera->w;
};

void camera_update( camera_t* camera, const struct sprite_t* sprite )
{
    const double sprite_r = sprite_right( sprite );
    const double camera_right_boundary_size = camera->w * 0.75;
    if ( sprite_r > camera->x + camera_right_boundary_size )
    {
        camera->x = sprite_r - camera_right_boundary_size;
    }
    const double camera_left_boundary_size = camera->w * 0.25;
    if ( sprite->x < camera->x + camera_left_boundary_size )
    {
        camera->x = sprite->x - camera_left_boundary_size;
    }

    const double sprite_b = sprite_bottom( sprite );
    const double camera_bottom_boundary_size = camera->h * 0.666;
    if ( sprite_b > camera->y + camera_bottom_boundary_size )
    {
        camera->y = sprite_b - camera_bottom_boundary_size;
    }
    const double camera_top_boundary_size = camera->h * 0.2;
    if ( sprite->y < camera->y + camera_top_boundary_size )
    {
        camera->y = sprite->y - camera_top_boundary_size;
    }
};
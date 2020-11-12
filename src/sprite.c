#include "camera.h"
#include "config.h"
#include "color.h"
#include "graphics.h"
#include "input.h"
#include "inventory.h"
#include "map.h"
#include "math.h"
#include "rect.h"
#include "render.h"
#include "sprite.h"

sprite_t sprite_create
(
    double x,
    double y,
    double w,
    double h
)
{
    graphics_t graphics = { GRAPHICS_REGULAR, {{ render_get_texture_id( "sprites/autumn.png" ), { 0.0, 0.0, 16.0, 24.0 }, { x, y, 16.0, 24.0 }, FLIP_NONE, 0.0 } }};
    sprite_t sprite =
    {
        { x, y, w, h },
        0.0, 0.0, 0.0, 0.0,
        SSTATE_FALLING,
        0,
        render_add_graphics( &graphics )
    };
    return sprite;
};

double sprite_bottom( const sprite_t * sprite )
{
    return sprite->position.y + sprite->position.h;
};

double sprite_right( const sprite_t * sprite )
{
    return sprite->position.x + sprite->position.w;
};

double sprite_center_x( const sprite_t * sprite )
{
    return sprite->position.x + sprite->position.w / 2.0;
};

double sprite_center_y( const sprite_t * sprite )
{
    return sprite->position.y + sprite->position.h / 2.0;
};

void sprite_update( sprite_t * sprite, struct map_t * map, const struct camera_t * camera )
{
    if ( sprite->state == SSTATE_ON_GROUND )
    {
        sprite->state = SSTATE_FALLING;
    }
    double prev_y = sprite->position.y;
    double prev_x = sprite->position.x;

    if ( input_held( INPUT_RIGHT ) )
    {
        sprite->accx = 0.25;
    }
    else if ( input_held( INPUT_LEFT ) )
    {
        sprite->accx = -0.25;
    }
    else
    {
        sprite->accx = 0.0;
    }

    sprite->vx += sprite->accx;
    if ( sprite->vx > 2.0 )
    {
        sprite->vx = 2.0;
    }
    if ( sprite->vx < -2.0 )
    {
        sprite->vx = -2.0;
    }

    if ( sprite->accx == 0.0 )
    {
        sprite->vx /= 1.15;
    }

    sprite->position.x += sprite->vx;

    if ( sprite->state == SSTATE_JUMPING )
    {
        sprite->accy = -0.5;
    }
    else if ( sprite->state == SSTATE_FALLING )
    {
        sprite->accy = 0.25;
    }
    else if ( sprite->state == SSTATE_ON_LADDER )
    {
        sprite->accy = ( input_held( INPUT_JUMP ) ) ? -2.0
            : ( input_held( INPUT_UP ) ) ? -0.25
            : ( input_held( INPUT_DOWN ) ) ? 0.25
            : 0.0;
    }
    sprite->vy += sprite->accy;
    if ( sprite->state == SSTATE_ON_LADDER )
    {
        if ( sprite->accy == 0.0 )
        {
            sprite->vy /= 1.15;
        }

        const double max = ( input_held( INPUT_JUMP ) ) ? 2.0 : 1.0;
        if ( sprite->vy > max )
        {
            sprite->vy = max;
        }
        else if ( sprite->vy < -1.0 * max )
        {
            sprite->vy = -1.0 * max;
        }
    }
    else if ( sprite->state == SSTATE_JUMPING && ( sprite->vy < -4.5 || !input_held( INPUT_JUMP ) ) )
    {
        sprite->state = SSTATE_FALLING;
    }
    else if ( sprite->vy > 4.0 )
    {
        sprite->vy = 4.0;
    }
    sprite->position.y += sprite->vy;

    if ( sprite->position.x < 0.0 || sprite_right( sprite ) > BLOCKS_TO_PIXELS( map->width ) )
    {
        sprite->accx = 0.0;
        sprite->vx *= -0.5;
        sprite->position.x = prev_x;
    }

    const int center_x_block = ( int )( floor( ( sprite_center_x( sprite ) ) / ( double )( BLOCK_SIZE ) ) );
    const int center_y_block = ( int )( floor( ( sprite_center_y( sprite ) ) / ( double )( BLOCK_SIZE ) ) );
    const int left_x = ( int )( floor( sprite->position.x / ( double )( BLOCK_SIZE ) ) );
    const int right_x = ( int )( floor( sprite_right( sprite ) / ( double )( BLOCK_SIZE ) ) );
    const int x_bottom = ( int )( floor( ( sprite_bottom( sprite ) - 5.0 ) / ( double )( BLOCK_SIZE ) ) );
    const int x_top = ( int )( floor( ( sprite->position.y + 5.0 ) / ( double )( BLOCK_SIZE ) ) );
    if ( map_test_pixel_gem_collision( map, left_x, x_bottom ) )
    {
        inventory_add_gems( 100 );
        map_remove( map, left_x, x_bottom );
    }
    if ( map_test_pixel_gem_collision( map, right_x, x_bottom ) )
    {
        inventory_add_gems( 100 );
        map_remove( map, right_x, x_bottom );
    }
    if ( map_test_pixel_gem_collision( map, left_x, x_top ) )
    {
        inventory_add_gems( 100 );
        map_remove( map, left_x, x_top );
    }
    if ( map_test_pixel_gem_collision( map, left_x, x_bottom ) )
    {
        inventory_add_gems( 100 );
        map_remove( map, right_x, x_top );
    }

    if
    (
        map_test_pixel_solid_collision( map, left_x, x_bottom ) || map_test_pixel_solid_collision( map, right_x, x_bottom ) ||
        map_test_pixel_solid_collision( map, left_x, x_top ) || map_test_pixel_solid_collision( map, right_x, x_top )
    )
    {
        sprite->accx = 0.0;
        sprite->vx *= -0.5;
        sprite->position.x = prev_x;
    }

    const int y_left_block_x = ( int )( floor( ( sprite->position.x + 2.0 ) / ( double )( BLOCK_SIZE ) ) );
    const int y_right_block_x = ( int )( floor( ( sprite_right( sprite ) - 2.0 ) / ( double )( BLOCK_SIZE ) ) );
    const int bottom_block_y = ( int )( floor( sprite_bottom( sprite ) / ( double )( BLOCK_SIZE ) ) );
    const int top_block_y = ( int )( floor( sprite->position.y / ( double )( BLOCK_SIZE ) ) );
    if ( map_test_pixel_solid_collision( map, y_left_block_x, bottom_block_y ) || map_test_pixel_solid_collision( map, y_right_block_x, bottom_block_y ) )
    {
        sprite->accy = 0.0;
        sprite->vy = 0.0;
        const int collision = sprite_bottom( sprite ) - ( bottom_block_y * BLOCK_SIZE );
        sprite->position.y -= collision;
        sprite->state = SSTATE_ON_GROUND;
    }
    else if ( map_test_pixel_top_solid_collision( map, y_left_block_x, bottom_block_y ) || map_test_pixel_top_solid_collision( map, y_right_block_x, bottom_block_y ) )
    {
        if ( sprite->vy >= 0.0 && sprite_bottom( sprite ) < bottom_block_y * BLOCK_SIZE + 4.0 )
        {
            sprite->accy = 0.0;
            sprite->vy = 0.0;
            const int collision = sprite_bottom( sprite ) - ( bottom_block_y * BLOCK_SIZE );
            sprite->position.y -= collision;
            sprite->state = SSTATE_ON_GROUND;
        }
    }
    else
    {
        if ( !map_test_pixel_ladder_collision( map, y_left_block_x, center_y_block ) && !map_test_pixel_ladder_collision( map, y_right_block_x, center_y_block ) )
        {
            if ( map_test_pixel_ladder_collision( map, y_left_block_x, bottom_block_y ) || map_test_pixel_ladder_collision( map, y_right_block_x, bottom_block_y ) )
            {
                if ( input_held( INPUT_DOWN ) )
                {
                    sprite->state = SSTATE_ON_LADDER;
                    sprite->accy = 0.0;
                    sprite->vy = 0.0;
                    sprite->position.y += 6.0;
                }
                else
                {
                    sprite->accy = 0.0;
                    sprite->vy = 0.0;
                    const int collision = sprite_bottom( sprite ) - ( bottom_block_y * BLOCK_SIZE );
                    sprite->position.y -= collision;
                    sprite->state = SSTATE_ON_GROUND;
                }
            }
        }

        if ( map_test_pixel_solid_collision( map, y_left_block_x, top_block_y ) || map_test_pixel_solid_collision( map, y_right_block_x, top_block_y ) )
        {
            sprite->accy = 0.0;
            sprite->vy *= -0.5;
            sprite->position.y = prev_y;
        }
    }

    if ( sprite->state == SSTATE_ON_GROUND || sprite->state == SSTATE_ON_LADDER )
    {
        if ( input_held( INPUT_JUMP ) )
        {
            if ( !sprite->jump_lock )
            {
                sprite->state = SSTATE_JUMPING;
            }
        }
        else
        {
            sprite->jump_lock = 0;
        }
    }
    else
    {
        sprite->jump_lock = 1;
    }

    if ( sprite->state == SSTATE_ON_LADDER )
    {
        if ( !map_test_pixel_ladder_collision( map, center_x_block, center_y_block ) )
        {
            if ( map_test_pixel_ladder_collision( map, y_left_block_x, bottom_block_y ) || map_test_pixel_ladder_collision( map, y_right_block_x, bottom_block_y ) )
            {
                sprite->state = SSTATE_ON_GROUND;
            }
            else
            {
                sprite->state = SSTATE_FALLING;
            }
        }
    }
    else if ( input_held( INPUT_UP ) )
    {
        if ( inventory_has_treasure( TREASURE_GLOVES ) && map_test_pixel_ladder_collision( map, center_x_block, center_y_block ) )
        {
            sprite->state = SSTATE_ON_LADDER;
            sprite->accy = 0.0;
            sprite->vy = 0.0;
        }
        else if ( map_test_pixel_treasure_collision( map, center_x_block, center_y_block ) )
        {
            map_remove( map, center_x_block, center_y_block );
            inventory_get_treasure( center_x_block );
        }
    }

    graphics_data_regular_t * graphics = &render_get_graphics( sprite->graphics_id )->data.regular;
    graphics->dest = camera_relative( camera, sprite->position );
};
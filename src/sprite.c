#include "input.h"
#include "render.h"
#include "game_state.h"
#include "sprite.h"
#include <stddef.h>
#include "unit.h"

static int sprite_test_collide_bottom( const Sprite * sprite, const Map * map, float bottom );
static int sprite_test_bottom_collide_type( int collide, int bottom, int jump, float vy );

int sprite_on_ground( Sprite * sprite, Map * map )
{
    int collide = sprite_test_collide_bottom( sprite, map, RECT_BOTTOM_DIR( sprite->position ) );
    return collide == 1 || collide == 2;
};

Sprite hero_create( int state_number )
{
    Sprite hero;
    hero.position.x = 32.0;
    hero.position.y = 32.0;
    hero.position.w = 16.0;
    hero.position.h = 25.0;
    hero.accx = 0.0;
    hero.accy = 0.0;
    hero.vx = 0.0;
    hero.vy = 0.0;
    hero.top_speed = 2.0;
    hero.start_speed = 0.25;
    hero.dir = DIR_RIGHT;
    hero.jump = 0;

    hero.gfx.id = render_add_graphic
    (
        graphic_create_sprite
        (
            render_get_texture_id( "sprites/autumn.png" ),
            0,
            hero.position,
            0.0,
            0.0,
            NULL
        ),
        state_number,
        LAYER_BEFORE_SPRITES_1
    );

    //printf( "SPRITE GFX: %d\n", hero.gfx.id );

    hero.gfx.walk_frame = 0;
    hero.gfx.walk_timer = 0;

    Graphic * gfx = render_get_graphic( hero.gfx.id );
    gfx->data.sprite.rotation_x = 180.0f;
    return hero;
}

void hero_update( Sprite * hero, void * level_state )
{
    Map * map = &(( LevelState * )( level_state ))->map;
    SpriteGraphics * gfx = &render_get_graphic( hero->gfx.id )->data.sprite;

    float prevx = hero->position.x;
    float prevy = hero->position.y;

    if ( input_held_left() )
    {
        hero->accx = -hero->start_speed;
        hero->dir = DIR_LEFT;
    }
    else if ( input_held_right() )
    {
        hero->accx = hero->start_speed;
        hero->dir = DIR_RIGHT;
    }
    else
    {
        hero->accx = 0.0f;
        hero->vx /= 1.15f;
    }
    if ( input_pressed_jump() && sprite_on_ground( hero, map ) )
    {
        hero->jump = 1;
        hero->accy = -0.75f;
    }
    else if ( !input_held_jump() )
    {
        hero->jump = 0;
    }

    hero->vx += hero->accx;
    if ( hero->vx > hero->top_speed )
    {
        hero->vx = hero->top_speed;
    }
    else if ( hero->vx < -hero->top_speed )
    {
        hero->vx = -hero->top_speed;
    }
    hero->position.x += hero->vx;

    if ( !hero->jump )
    {
        if ( !sprite_on_ground( hero, map ) )
        {
            hero->accy = 0.25f;
        }
        else
        {
            hero->accy = hero->vy = 0.0f;
        }
    }

    hero->vy += hero->accy;

    if ( hero->vy > 4.0f )
    {
        hero->vy = 4.0f;
    }
    else if ( hero->vy < -5.0f )
    {
        hero->jump = 0;
    }
    hero->position.y += hero->vy;

    if ( hero->position.x < 0.0f )
    {
        hero->position.x = 0.0f;
        hero->vx *= -0.25f;
    }
    else if ( RECT_RIGHT_DIR( hero->position ) > BLOCKS_TO_PIXELS( map->w ) )
    {
        hero->position.x = BLOCKS_TO_PIXELS( map->w ) - hero->position.w;
        hero->vx *= -0.25f;
    }

    gfx->dest = hero->position;
    switch ( hero->dir )
    {
        case ( DIR_LEFT ):
        {
            if ( gfx->rotation_x > 0.0 )
            {
                gfx->rotation_x -= 5.0;
            }
        }
        break;
        case ( DIR_RIGHT ):
        {
            if ( gfx->rotation_x < 180.0 )
            {
                gfx->rotation_x += 5.0;
            }
        }
        break;
    }
    if ( sprite_on_ground( hero, map ) )
    {
        if ( hero->accx == 0.0f )
        {
            gfx->src.x = 0.0f;
        }
        else
        {
            if ( hero->gfx.walk_timer == 7 )
            {
                hero->gfx.walk_timer = 0;
                ++hero->gfx.walk_frame;
                if ( hero->gfx.walk_frame == 4 )
                {
                    hero->gfx.walk_frame = 0;
                }
                gfx->src.x =
                    ( hero->gfx.walk_frame == 1 ) ? 16.0f
                    : ( hero->gfx.walk_frame == 3 ) ? 32.0f
                    : 0.0f;
            }
            else
            {
                ++hero->gfx.walk_timer;
            }
        }
    }
    else
    {
        gfx->src.x = 48.0f;
    }


    {
        const int left_tilex = ( int )( floor( hero->position.x / PIXELS_PER_BLOCK ) );
        const int right_tilex = ( int )( floor( RECT_RIGHT_DIR( hero->position ) / PIXELS_PER_BLOCK ) );
        const int bottom_tiley = ( int )( floor( ( RECT_BOTTOM_DIR( hero->position ) - 5.0 ) / PIXELS_PER_BLOCK ) );
        const int top_tiley = ( int )( floor( ( hero->position.y + 5.0 ) / PIXELS_PER_BLOCK ) );

        for ( int i = 0; i < map->num_o_collision_layers; ++i )
        {
            const int left_top_collide = map->collision[ i ][ ( map->w * top_tiley ) + left_tilex ];
            const int left_bottom_collide = map->collision[ i ][ ( map->w * bottom_tiley ) + left_tilex ];
            if ( left_top_collide == 1 || left_bottom_collide == 1 )
            {
                hero->position.x = ( ceil( hero->position.x / 16.0f ) * 16.0f );
                hero->vx *= -0.5f;
                hero->accx = 0.0f;
            }

            const int right_top_collide = map->collision[ i ][ ( map->w * top_tiley ) + right_tilex ];
            const int right_bottom_collide = map->collision[ i ][ ( map->w * bottom_tiley ) + right_tilex ];
            if ( right_top_collide == 1 || right_bottom_collide == 1 )
            {
                hero->position.x = ( floor( RECT_RIGHT_DIR( hero->position ) / 16.0f ) * 16.0f ) - hero->position.w;
                hero->vx *= -0.5f;
                hero->accx = 0.0f;
            }
        }
    }

    {
        const int left_tilex = ( int )( floor( ( hero->position.x + 2.0f ) / PIXELS_PER_BLOCK ) );
        const int right_tilex = ( int )( floor( ( RECT_RIGHT_DIR( hero->position ) - 2.0f ) / PIXELS_PER_BLOCK ) );
        const int top_tiley = ( int )( floor( ( hero->position.y ) / PIXELS_PER_BLOCK ) );

        for ( int i = 0; i < map->num_o_collision_layers; ++i )
        {
            const int left_collide = map->collision[ i ][ ( map->w * top_tiley ) + left_tilex ];
            const int right_collide = map->collision[ i ][ ( map->w * top_tiley ) + right_tilex ];
            if ( left_collide == 1 || right_collide == 1 )
            {
                hero->position.y = ( ceil( hero->position.y / 16.0f ) * 16.0f );
                hero->vy *= -0.5f;
                hero->accy = 0.0f;
                hero->jump = 0;
            }
        }
    }

    int bottom_collide = sprite_test_collide_bottom( hero, map, RECT_BOTTOM_DIR( hero->position ) - 1.0 );
    if ( bottom_collide == 1 || bottom_collide == 2 )
    {
        hero->position.y = ( floor( RECT_BOTTOM_DIR( hero->position ) / 16.0f ) * 16.0f ) - hero->position.h;
        hero->vy = 0.0f;
        hero->accy = 0.0f;
    }

    render_adjust_camera( &hero->position, ( float )( BLOCKS_TO_PIXELS( map->w ) ), ( float )( BLOCKS_TO_PIXELS( map->h ) ) );
};

static int sprite_test_collide_bottom( const Sprite * sprite, const Map * map, float bottom )
{
    const int bottom_tiley = ( int )( floor( bottom / PIXELS_PER_BLOCK ) );
    if ( bottom_tiley >= map->h ) return 0;

    const int left_tilex = ( int )( floor( ( sprite->position.x + 2.0f ) / PIXELS_PER_BLOCK ) );
    const int right_tilex = ( int )( floor( ( RECT_RIGHT_DIR( sprite->position ) - 2.0f ) / PIXELS_PER_BLOCK ) );

    for ( int i = 0; i < map->num_o_collision_layers; ++i )
    {
        if ( left_tilex >= 0 )
        {
            int test_collide = sprite_test_bottom_collide_type( map->collision[ i ][ ( map->w * bottom_tiley ) + left_tilex ], ( int )( bottom ), sprite->jump, sprite->vy );
            if ( test_collide != 0 ) return test_collide;
        }

        if ( right_tilex < map->w )
        {
            int test_collide = sprite_test_bottom_collide_type( map->collision[ i ][ ( map->w * bottom_tiley ) + right_tilex ], ( int )( bottom ), sprite->jump, sprite->vy );
            if ( test_collide != 0 ) return test_collide;
        }
    }

    return 0;
};

static int sprite_test_bottom_collide_type( int collide, int bottom, int jump, float vy )
{
    if ( collide > 0 )
    {
        const int bottom_rel = bottom % 16;
        const int target_bottom_rel = ( vy > 0.0f ) ? 5 : 2;
        return ( collide == 2 )
            ? ( ( !jump && bottom_rel < target_bottom_rel && bottom_rel != 0 ) ? 2 : 0 )
            : collide;
    }
    return 0;
};
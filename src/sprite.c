#include "input.h"
#include "render.h"
#include "game_state.h"
#include "sprite.h"
#include <stddef.h>
#include "unit.h"

int sprite_on_ground( Sprite * sprite, Map * map )
{
    return SPRITE_BOTTOM( sprite ) >= ( float )( WINDOW_HEIGHT_PIXELS - 16 );
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
    else if ( hero->vy < -4.0f )
    {
        hero->jump = 0;
    }
    hero->position.y += hero->vy;

    if ( SPRITE_BOTTOM( hero ) > ( float )( WINDOW_HEIGHT_PIXELS - 16 ) )
    {
        hero->position.y = ( float )( WINDOW_HEIGHT_PIXELS - 16 ) - hero->position.h;
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
};
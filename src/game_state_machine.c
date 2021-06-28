#include "assoc_array.h"
#include "color.h"
#include "game_state_machine.h"
#include "graphic.h"
#include "input.h"
#include "log.h"
#include "map.h"
#include "render.h"
#include "sprite.h"
#include <stddef.h>
#include <stdlib.h>
#include "text.h"

GameState states[ MAX_STATES ];
int number_of_states = 0;

static void state_close_single( int number );
static void state_init_single( int number );

void state_init()
{
    state_change( state_create_title() );
};

void state_update()
{
    switch ( states[ number_of_states - 1 ].type )
    {
        case ( STATE_TITLE ):
        {
            if ( input_pressed_confirm() )
            {
                state_change( state_create_level() );
            }
        }
        break;
        case ( STATE_LEVEL ):
        {
            hero_update( &states[ number_of_states - 1 ].data.level.hero, &states[ number_of_states - 1 ].data.level );
            if ( input_pressed_menu() )
            {
                state_push( state_create_pause() );
            }
        }
        break;
        case ( STATE_PAUSE ):
        {
            if ( input_pressed_confirm() )
            {
                state_change( state_create_title() );
            }
            else if ( input_pressed_cancel() || input_pressed_menu() )
            {
                state_pop();
            }
        }
        break;
        case ( STATE_ERROR ):
        {
            if ( input_pressed_confirm() )
            {
                exit( -1 );
            }
        }
        break;
    }
};

void state_change( GameState state )
{
    while ( number_of_states > 0 )
    {
        state_close_single( --number_of_states );
    }
    number_of_states = 1;
    render_set_states_number( number_of_states );
    render_clear_graphics();
    states[ 0 ] = state;
    state_init_single( 0 );
};

void state_push( GameState state )
{
    states[ number_of_states ] = state;
    render_set_states_number( number_of_states );
    state_init_single( number_of_states );
    ++number_of_states;
};

void state_pop()
{
    state_close_single( --number_of_states );
    render_clear_state_graphics( number_of_states );
    render_set_states_number( number_of_states );
};

GameState state_create_title()
{
    GameState s;
    s.type = STATE_TITLE;
    return s;
};

GameState state_create_level()
{
    GameState s;
    s.type = STATE_LEVEL;
    return s;
};

GameState state_create_pause()
{
    GameState s;
    s.type = STATE_PAUSE;
    return s;
};

GameState state_create_error( const char * msg )
{
    GameState s;
    s.type = STATE_ERROR;
    s.data.error.msg = msg;
    return s;
};

static void state_close_single( int number )
{
    switch ( states[ number ].type )
    {
        case ( STATE_TITLE ):
        {
        }
        break;
        case ( STATE_LEVEL ):
        {
            map_destroy( &states[ number ].data.level.map );
        }
        break;
        case ( STATE_PAUSE ):
        {
        }
        break;
    }
};

static void state_init_single( int number )
{
    switch ( states[ number ].type )
    {
        case ( STATE_TITLE ):
        {
            Color color = { 255.0, 255.0, 255.0, 255.0 };
            render_add_graphic( graphic_create_full_rect( color ), number, LAYER_BG_1 );
            AssocArray args = assoc_array_create( -1 );
            assoc_array_add( &args, "y", value_create_float( 16.0 ) );
            assoc_array_add( &args, "align", value_create_int( ALIGN_CENTER ) );
            render_add_graphic( graphic_create_text( text_create( "Boskeopolis World", &args ) ), number, LAYER_FG_1 );
            assoc_array_destroy( &args );
        }
        break;
        case ( STATE_LEVEL ):
        {
            states[ number ].data.level.hero = hero_create( number );
            int map_error = map_create( &states[ number ].data.level.map, number );
            if ( map_error != 0 )
            {
                state_change( state_create_error( log_get_error() ) );
            }
        }
        break;
        case ( STATE_PAUSE ):
        {
            Rect rect = { 32.0, 32.0, 320.0, 128.0 };
            Color color = { 255.0, 0.0, 0.0, 255.0 };
            render_add_graphic( graphic_create_rect( rect, color ), number, LAYER_BG_1 );
        }
        break;
        case ( STATE_ERROR ):
        {
            Color bg = { 0.0f, 0.0f, 0.0f, 255.0f };
            render_add_graphic( graphic_create_full_rect( bg ), number, LAYER_BG_1 );
            Color * color = calloc( 1, sizeof( Color ) );
            color->r = color->g = color->b = color->a = 255.0f;
            AssocArray args = assoc_array_create( -1 );
            assoc_array_add( &args, "align", value_create_int( ALIGN_CENTER ) );
            assoc_array_add( &args, "valign", value_create_int( VALIGN_MIDDLE ) );
            assoc_array_add( &args, "color", value_create_unique_ptr( color ) );
            render_add_graphic( graphic_create_text( text_create( states[ number ].data.error.msg, &args ) ), number, LAYER_FG_1 );
            assoc_array_destroy( &args );
        }
        break;
    }
};
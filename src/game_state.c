#include "color.h"
#include "config.h"
#include "game_state.h"
#include "input.h"
#include "inventory.h"
#include "rect.h"
#include "render.h"

#include "camera.h"
#include "inventory.h"
#include "map.h"
#include "sprite.h"

#define MAX_STATES 3

static game_state_t states[ MAX_STATES ];
static int number_of_states = 0;

static void game_state_destroy( game_state_t * state );

void game_state_init()
{
    for ( int i = 1; i < MAX_STATES; ++i )
    {
        states[ i ].type = GSTATE_NULL;
    }
    number_of_states = 1;
};

void game_state_pop()
{
    game_state_destroy( &states[ --number_of_states ] );
    input_reset();
};

void game_state_change_level()
{
    game_state_destroy_all();
    states[ 0 ].timer = 0;
    states[ 0 ].type = GSTATE_LEVEL;
    states[ 0 ].data.level.camera.position.x = ( double )( BLOCKS_TO_PIXELS( 120 ) );
    states[ 0 ].data.level.camera.position.y = ( double )( BLOCKS_TO_PIXELS( 17 ) );
    states[ 0 ].data.level.camera.position.w = ( double )( WINDOW_WIDTH_PIXELS );
    states[ 0 ].data.level.camera.position.h = ( double )( WINDOW_HEIGHT_PIXELS );
    map_create( &states[ 0 ].data.level );
    states[ 0 ].data.level.player = sprite_create( ( double )( BLOCKS_TO_PIXELS( 144 ) ), ( double )( BLOCKS_TO_PIXELS( 26 ) ), 16.0, 24.0 );
    number_of_states = 1;
    input_reset();
};

void game_state_add_treasure_message( treasure_t treasure )
{
    states[ number_of_states ].type = GSTATE_MESSAGE;
    ++number_of_states;
    input_reset();
};

void game_state_update()
{
    if ( number_of_states > 0 )
    {
        switch ( states[ number_of_states - 1 ].type )
        {
            case ( GSTATE_TITLE ):
            {
                game_state_change_level();
            }
            break;
            case ( GSTATE_LEVEL ):
            {
                game_state_level_data_t * data = &states[ number_of_states - 1 ].data.level;
                sprite_update( &data->player, &data->extra, &data->camera );
                map_update( &data->extra, &data->camera, states[ number_of_states - 1 ].timer );
                camera_update( &data->camera, &data->player, &data->extra );
                inventory_update();
            }
            break;
            case ( GSTATE_MESSAGE ):
            {
                if ( input_held( INPUT_JUMP ) )
                {
                    game_state_pop();
                }
            }
            break;
        }
        ++states[ number_of_states - 1 ].timer;
    }
};

void game_state_destroy_all()
{
    for ( int i = 0; i < number_of_states; ++i )
    {
        game_state_destroy( &states[ i ] );
    }
};

static void game_state_destroy( game_state_t * state )
{
    switch ( state->type )
    {
        case ( GSTATE_TITLE ):
        {
        }
        break;
        case ( GSTATE_LEVEL ):
        {
        }
        break;
        case ( GSTATE_MESSAGE ):
        {
        }
        break;
    }
};
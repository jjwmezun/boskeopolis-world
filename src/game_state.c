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

typedef enum
{
    GSTATE_TITLE,
    GSTATE_LEVEL,
    GSTATE_MESSAGE,
    GSTATE_NULL
} game_state_type_t;

typedef struct game_state_level_data_t
{
    camera_t camera;
    map_t map;
    sprite_t player;
} game_state_level_data_t;

typedef union game_state_data_t
{
    game_state_level_data_t level;
} game_state_data_t;

typedef struct game_state_t
{
    game_state_type_t type;
    game_state_data_t data;
} game_state_t;

static game_state_t states[ MAX_STATES ];
static int number_of_states = 0;

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
    --number_of_states;
    input_reset();
};

void game_state_change_level()
{
    states[ 0 ].type = GSTATE_LEVEL;
    states[ 0 ].data.level.camera.x = ( double )( BLOCKS_TO_PIXELS( 120 ) );
    states[ 0 ].data.level.camera.y = ( double )( BLOCKS_TO_PIXELS( 17 ) );
    states[ 0 ].data.level.camera.w = ( double )( WINDOW_WIDTH_PIXELS );
    states[ 0 ].data.level.camera.h = ( double )( WINDOW_HEIGHT_PIXELS );
    states[ 0 ].data.level.map = map_create();
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
                if ( input_held( INPUT_JUMP ) )
                {
                    game_state_change_level();
                }
            }
            break;
            case ( GSTATE_LEVEL ):
            {
                game_state_level_data_t * data = &states[ number_of_states - 1 ].data.level;
                sprite_update( &data->player, &data->map, &data->camera );
                camera_update( &data->camera, &data->player );
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
    }
};
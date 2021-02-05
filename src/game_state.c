#include "color.h"
#include "config.h"
#include "game_state.h"
#include "graphics.h"
#include "input.h"
#include "inventory.h"
#include "rect.h"
#include "render.h"

#include "camera.h"
#include "inventory.h"
#include "map.h"
#include "sprite.h"
#include "text.h"

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

    states[ 0 ].type = GSTATE_TITLE;
    const int texture_id = render_create_custom_texture( "title", WINDOW_WIDTH_PIXELS, WINDOW_HEIGHT_PIXELS );
    render_set_target_texture( texture_id );
    render_clear();
    color_t bg = { 255, 255, 255, 255 };
    render_color_canvas( &bg );

    text_args_t title_args = text_default_args();
    title_args.y = 16.0;
    title_args.x_padding = 16.0;
    title_args.align = ALIGN_CENTER;
    text_t title = text_create( "¡BOSKEOPOLIS WORLD!", title_args );
    render_text( &title );

    text_args_t desc_args = text_default_args();
    desc_args.y = 64.0;
    desc_args.x_padding = 16.0;
    text_t desc = text_create( "Boskeopolis is an obscure city-state in the Verdazul archipelago in Orange Ocean ( or, as other countries call it, the Pacific Ocean ).", desc_args );
    render_text( &desc );

    text_args_t desc2_args = text_default_args();
    desc2_args.x_padding = 16.0;
    desc2_args.y_padding = 16.0;
    desc2_args.align = ALIGN_RIGHT;
    desc2_args.valign = VALIGN_BOTTOM;
    desc2_args.color.r = 255;
    text_t desc2 = text_create( "Boskeopolis is an obscure city-state in the Verdazul archipelago in Orange Ocean ( or, as other countries call it, the Pacific Ocean ).", desc2_args );
    render_text( &desc2 );

    graphics_t graphics = { GRAPHICS_REGULAR, LAYER_BG1, {{ texture_id, { 0, 0, WINDOW_WIDTH_PIXELS, WINDOW_HEIGHT_PIXELS }, { 0, 0, WINDOW_WIDTH_PIXELS, WINDOW_HEIGHT_PIXELS }, FLIP_NONE, 0.0 }}};
    render_add_graphics( &graphics );
    render_release_target_texture();
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
                if ( input_held( INPUT_JUMP ) )
                {
                    game_state_change_level();
                }
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
    render_clear_graphics();
};
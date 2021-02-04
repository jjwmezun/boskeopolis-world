#include "asset.h"
#include "color.h"
#include "config.h"
#include "engine.h"
#include "game_state.h"
#include "input.h"
#include "inventory.h"
#include "render.h"

#define DT 17

static int running = 1;
static int ticks = 0;
static int accumulator = 0;

int main( int argc, char ** argv )
{
    // Init
    if ( engine_init() != 0 )
    {
        return 1;
    }
    if ( render_init( "Boskeopolis World", WINDOW_WIDTH_PIXELS, WINDOW_HEIGHT_PIXELS ) != 0 )
    {
        return 1;
    }
    asset_init();
    input_init();
    inventory_init();
    game_state_init();

    // Execute
    ticks = engine_get_ticks();
    while ( running )
    {
        running = engine_handle_events();

        int new_time = engine_get_ticks();
        int frame_time = new_time - ticks;
        ticks = new_time;
        accumulator += frame_time;
        while ( accumulator >= DT )
        {
            game_state_update();
            accumulator -= DT;
        }

        render_execute();
    }

    // Close
    game_state_destroy_all();
    asset_close();
    render_close();
    engine_close();
    return 0;
}
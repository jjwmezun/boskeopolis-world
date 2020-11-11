#include "color.h"
#include "config.h"
#include "engine.h"
#include "game_state.h"
#include "input.h"
#include "inventory.h"
#include "render.h"

static int running = 1;
static int ticks;

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
    input_init();
    inventory_init();
    game_state_init();

    // Execute
    while ( running )
    {
        running = engine_handle_events();

        // Maintain framerate.
        const int relative_ticks = engine_get_ticks() - ticks;
		if ( relative_ticks < FPS_MILLISECONDS )
		{
            int delay = FPS_MILLISECONDS - relative_ticks;
            if ( delay > 0 )
            {
			    engine_delay( delay );
            }
		}

        game_state_update();

        render_execute();

        // Update time.
        ticks = engine_get_ticks();
    }

    // Close
    render_close();
    engine_close();
    return 0;
}
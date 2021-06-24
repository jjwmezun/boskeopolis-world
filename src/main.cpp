#include "color.hpp"
#include "engine.hpp"
#include "filename.hpp"
#include "game_state_machine.hpp"
#include "input.hpp"
#include "localization.hpp"
#include "log.hpp"
#include "rect.hpp"
#include "render.hpp"

#define DT 17

static int running = 1;
static int ticks = 0;
static int accumulator = 0;

int main()
{
    filename_init();
    if ( !engine_init() )
    {
        log_error( "¡Error! Failed to initialize game!\n" );
        return -1;
    }

    localization_init();
    state_init();

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
            state_update();
            input_update();
            accumulator -= DT;
        }

        render_update();
    }

    input_close();
    localization_close();
    render_close();
    engine_close();
    filename_close();
    return 0;
}
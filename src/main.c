#include "color.h"
#include "engine.h"
#include "log.h"
#include "rect.h"
#include "render.h"

#define DT 17

static int running = 1;
static int ticks = 0;
static int accumulator = 0;

int main()
{
    if ( !engine_init() )
    {
        log_error( "¡Error! Failed to initialize game!\n" );
        return -1;
    }


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
            accumulator -= DT;
        }

        render_update();
    }

    render_close();
    engine_close();
    return 0;
}
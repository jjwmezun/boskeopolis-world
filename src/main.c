#include "color.h"
#include "engine.h"
#include "log.h"
#include "rect.h"
#include "render.h"

#include <stdlib.h>

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

    Color c;
    c.r = 255.0f;
    c.g = 255.0f;
    c.b = 255.0f;
    c.a = 255.0f;
    render_add_graphic( graphic_create_full_rect( c ), 1, LAYER_BEFORE_BLOCKS_1 );
    int * tiles = ( int * )( calloc( 25 * 14, sizeof( int ) ) );
    tiles[ 5 ] = 4102;
    render_add_tilemap( "urban", tiles, 25, 14, 2, 1, LAYER_BLOCKS_1 );

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
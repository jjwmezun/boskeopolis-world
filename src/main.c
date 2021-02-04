#include "asset.h"
#include "color.h"
#include "config.h"
#include "engine.h"
#include "game_state.h"
#include "input.h"
#include "inventory.h"
#include "render.h"

#include "speed.h"

#define DT 17
#define MAX_TIMES 9999

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

    /*
    uint64_t times[ MAX_TIMES ];
    int i = 0;
    */

    ticks = engine_get_ticks();
    while ( running )
    {
        //uint64_t start_time = test_speed();
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
        /*
        if ( i < MAX_TIMES )
        {
            times[ i ] = test_speed() - start_time;
            ++i;
        }
        else
        {
            uint64_t average = 0;
            uint64_t max = 0;
            uint64_t min = 9999999999;
            for ( int j = 0; j < MAX_TIMES; ++j )
            {
                average += times[ j ];
                max = ( times[ j ] > max ) ? times[ j ] : max;
                min = ( times[ j ] < max ) ? times[ j ] : min;
            }
            average /= MAX_TIMES;
            printf( "AVERAGE: %ld\n", average );
            printf( "MAX: %ld\n", max );
            printf( "MIN: %ld\n", min );
            running = 0;
        }*/
    }

    // Close
    game_state_destroy_all();
    asset_close();
    render_close();
    engine_close();
    return 0;
}
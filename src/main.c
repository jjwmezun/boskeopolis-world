#include "camera.h"
#include "color.h"
#include "config.h"
#include "engine.h"
#include "input.h"
#include "inventory.h"
#include "map.h"
#include "rect.h"
#include "render.h"
#include "sprite.h"

const color_t background_color = { 0, 0, 0, 255 };
int running = 1;
int ticks;

int main( int argc, char** argv )
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
    camera_t camera = { ( double )( BLOCKS_TO_PIXELS( 120 ) ), ( double )( BLOCKS_TO_PIXELS( 17 ) ), ( double )( WINDOW_WIDTH_PIXELS ), ( double )( WINDOW_HEIGHT_PIXELS ) };
    map_t map = map_create();
    sprite_t sprite = { ( double )( BLOCKS_TO_PIXELS( 144 ) ), ( double )( BLOCKS_TO_PIXELS( 26 ) ), 16.0, 24.0, 0.0, 0.0, 0.0, 0.0, SSTATE_FALLING, 0 };

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

        // Update
        sprite_update( &sprite, &map );
        camera_update( &camera, &sprite );
        inventory_update();

        // Render
        render_start( &background_color );
        map_render( &map, &camera );
        sprite_render( &sprite, &camera );
        render_end();

        // Update time.
        ticks = engine_get_ticks();
    }

    // Close
    render_close();
    engine_close();
    return 0;
}
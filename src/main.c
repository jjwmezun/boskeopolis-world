#include "camera.h"
#include "config.h"
#include "input.h"
#include "inventory.h"
#include "map.h"
#include <math.h>
#include <SDL2/SDL.h>
#include "sprite.h"

SDL_Window* window;
SDL_Renderer* renderer;
int running = 1;
int ticks;

int min( int a, int b )
{
    return ( a > b ) ? b : a;
};

int main( int argc, char** argv )
{
    if ( SDL_Init( SDL_INIT_EVENTS | SDL_INIT_VIDEO ) != 0 )
    {
        SDL_Log( "Unable to initialize SDL: %s", SDL_GetError() );
        return 1;
    }
    window = SDL_CreateWindow
    (
        "Boskeopolis World",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH_PIXELS,
		WINDOW_HEIGHT_PIXELS,
		SDL_WINDOW_RESIZABLE
    );
    if ( window == NULL )
    {
        SDL_Log( "Unable to create window: %s", SDL_GetError() );
        return 1;
    }
    renderer = SDL_CreateRenderer( window, -1, 0 );
    if ( renderer == NULL )
    {
        SDL_Log( "Unable to create renderer: %s", SDL_GetError() );
        return 1;
    }
    input_init();
    camera_t camera = { ( double )( BLOCKS_TO_PIXELS( 120 ) ), ( double )( BLOCKS_TO_PIXELS( 17 ) ), ( double )( WINDOW_WIDTH_PIXELS ), ( double )( WINDOW_HEIGHT_PIXELS ) };
    map_t map = map_create();
    sprite_t sprite = { ( double )( BLOCKS_TO_PIXELS( 144 ) ), ( double )( BLOCKS_TO_PIXELS( 26 ) ), 16.0, 24.0, 0.0, 0.0, 0.0, 0.0, SSTATE_FALLING, 0 };

    while ( running )
    {
        SDL_Event event;
        while ( SDL_PollEvent( &event ) )
        {
            switch ( event.type )
            {
                case ( SDL_QUIT ):
                {
                    running = 0;
                }
                break;

                case ( SDL_KEYDOWN ):
                {
                    if ( event.key.keysym.sym == SDLK_ESCAPE )
                    {
                        running = 0;
                    }
                    else
                    {
                        input_register_press( event.key.keysym.sym );
                    }
                }
                break;

                case ( SDL_KEYUP ):
                {
                    input_register_release( event.key.keysym.sym );
                }
                break;
            }
        }

		if ( ( int )( SDL_GetTicks() ) - ticks < FPS_MILLISECONDS )
		{
            int delay = FPS_MILLISECONDS - ( ( int )( SDL_GetTicks() ) - ticks );
            if ( delay > 0 )
            {
			    SDL_Delay( delay );
            }
		}

        sprite_update( &sprite, &map );
        camera_update( &camera, &sprite );
        inventory_update();

        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
        SDL_RenderClear( renderer );

        const int start_y = PIXELS_TO_BLOCKS( ( int )( camera.y ) );
        const int start_x = PIXELS_TO_BLOCKS( ( int )( camera.x ) );
        const int end_y = min( MAP_HEIGHT, PIXELS_TO_BLOCKS( ( int )( camera_bottom( &camera ) ) ) );
        const int end_x = min( MAP_WIDTH, PIXELS_TO_BLOCKS( ( int )( camera_right( &camera ) ) ) );
        for ( int y = start_y; y < end_y; ++y )
        {
            for ( int x = start_x; x < end_x; ++x )
            {
                if ( map.tiles[ MAP_INDEX( x, y ) ] )
                {
                    const SDL_Rect block_orig = { x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
                    const SDL_Rect block = camera_relative( &camera, block_orig );
                    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
                    SDL_RenderFillRect( renderer, &block );
                }
            }
        }
        sprite_render( &sprite, &camera, renderer );

        SDL_RenderPresent( renderer );

        ticks = SDL_GetTicks();
    }
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    SDL_Quit();
    return 0;
}
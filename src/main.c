#include "config.h"
#include "input.h"
#include <math.h>
#include <SDL2/SDL.h>

SDL_Window* window;
SDL_Renderer* renderer;
int running = 1;
int ticks;

const int area[ WINDOW_HEIGHT_BLOCKS ][ WINDOW_WIDTH_BLOCKS ] =
{
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};

typedef struct player_t
{
    double x;
    double y;
    double w;
    double h;
    double accx;
    double accy;
    double vx;
    double vy;
    int on_ground;
    int is_jumping;
    int jump_lock;
} player_t;

player_t player = { 96.0, 32.0, 16.0, 24.0, 0.0, 0.0, 0.0, 0.0, 0, 0, 0 };

void player_render()
{
    const SDL_Rect coords = { ( int )( player.x ), ( int )( player.y ), ( int )( player.w ), ( int )( player.h ) };
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
    SDL_RenderFillRect( renderer, &coords );
};

#define PLAYER_BOTTOM( player ) ( player.y + player.h )
#define PLAYER_RIGHT( player ) ( player.x + player.w )

int player_test_pixel_collision( int x, int y )
{
    return
        x > 0 &&
        x < WINDOW_WIDTH_BLOCKS &&
        y > 0 &&
        y < WINDOW_HEIGHT_BLOCKS &&
        area[ y ][ x ];
};

void player_update()
{
    player.on_ground = 0;
    double prev_y = player.y;
    double prev_x = player.x;

    if ( input_held( INPUT_RIGHT ) )
    {
        player.accx = 0.25;
    }
    else if ( input_held( INPUT_LEFT ) )
    {
        player.accx = -0.25;
    }
    else
    {
        player.accx = 0.0;
    }

    player.vx += player.accx;
    if ( player.vx > 2.0 )
    {
        player.vx = 2.0;
    }
    if ( player.vx < -2.0 )
    {
        player.vx = -2.0;
    }

    if ( player.accx == 0.0 )
    {
        player.vx /= 1.15;
    }

    player.x += player.vx;

    if ( player.is_jumping )
    {
        player.accy = -1.0;
    }
    else
    {
        player.accy = 0.25;
    }
    player.vy += player.accy;
    if ( player.is_jumping && ( player.vy < -5.0 || !input_held( INPUT_JUMP ) ) )
    {
        player.is_jumping = 0;
    }
    else if ( player.vy > 4.0 )
    {
        player.vy = 4.0;
    }
    player.y += player.vy;

    if ( player.x < 0.0 || PLAYER_RIGHT( player ) > WINDOW_WIDTH_PIXELS )
    {
        player.accx = 0.0;
        player.vx *= -0.5;
        player.x = prev_x;
    }

    const int left_x = ( int )( floor( player.x / ( double )( BLOCK_SIZE ) ) );
    const int right_x = ( int )( floor( PLAYER_RIGHT( player ) / ( double )( BLOCK_SIZE ) ) );
    const int x_bottom = ( int )( floor( ( PLAYER_BOTTOM( player ) - 5.0 ) / ( double )( BLOCK_SIZE ) ) );
    const int x_top = ( int )( floor( ( player.y + 5.0 ) / ( double )( BLOCK_SIZE ) ) );
    if
    (
        player_test_pixel_collision( left_x, x_bottom ) || player_test_pixel_collision( right_x, x_bottom ) ||
        player_test_pixel_collision( left_x, x_top ) || player_test_pixel_collision( right_x, x_top )
    )
    {
        player.accx = 0.0;
        player.vx *= -0.5;
        player.x = prev_x;
    }

    const int y_left_block_x = ( int )( floor( ( player.x + 2.0 ) / ( double )( BLOCK_SIZE ) ) );
    const int y_right_block_x = ( int )( floor( ( PLAYER_RIGHT( player ) - 2.0 ) / ( double )( BLOCK_SIZE ) ) );
    const int bottom_block_y = ( int )( floor( PLAYER_BOTTOM( player ) / ( double )( BLOCK_SIZE ) ) );
    const int top_block_y = ( int )( floor( player.y / ( double )( BLOCK_SIZE ) ) );
    if ( player_test_pixel_collision( y_left_block_x, bottom_block_y ) || player_test_pixel_collision( y_right_block_x, bottom_block_y ) )
    {
        player.accy = 0.0;
        player.vy = 0.0;
        const int collision = PLAYER_BOTTOM( player ) - ( bottom_block_y * BLOCK_SIZE );
        player.y -= collision;
        player.on_ground = 1;
    }
    else if ( player_test_pixel_collision( y_left_block_x, top_block_y ) || player_test_pixel_collision( y_right_block_x, top_block_y ) )
    {
        player.accy = 0.0;
        player.vy *= -0.5;
        player.y = prev_y;
    }

    if ( player.y > WINDOW_HEIGHT_PIXELS - 16.0 )
    {
        player.accy = 0.0;
        player.vy = 0.0;
        player.y = prev_y;
    }

    if ( player.on_ground )
    {
        if ( input_held( INPUT_JUMP ) )
        {
            if ( !player.jump_lock )
            {
                player.is_jumping = 1;
            }
        }
        else
        {
            player.jump_lock = 0;
        }
    }
    else
    {
        player.jump_lock = 1;
    }
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

        player_update();

        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
        SDL_RenderClear( renderer );

        for ( int y = 0; y < WINDOW_HEIGHT_BLOCKS; ++y )
        {
            for ( int x = 0; x < WINDOW_WIDTH_BLOCKS; ++x )
            {
                if ( area[ y ][ x ] )
                {
                    const SDL_Rect block = { x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
                    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
                    SDL_RenderFillRect( renderer, &block );
                }
            }
        }
        player_render();

        SDL_RenderPresent( renderer );

        ticks = SDL_GetTicks();
    }
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    SDL_Quit();
    return 0;
}
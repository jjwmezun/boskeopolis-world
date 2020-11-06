#include "color.h"
#include "render.h"
#include "rect.h"
#include <SDL2/SDL.h>

SDL_Window* window;
SDL_Renderer* renderer;

int render_init( const char* title, int width, int height )
{
    window = SDL_CreateWindow
    (
        title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
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
    return 0;
};

void render_start( const struct color_t* color )
{
    SDL_SetRenderDrawColor( renderer, color->r, color->g, color->b, color->a );
    SDL_RenderClear( renderer );
};

void render_end()
{
    SDL_RenderPresent( renderer );
};

void render_rect( const struct rect_t* rect, const struct color_t* color )
{
    SDL_Rect raw_rect = { rect->x, rect->y, rect->w, rect->h };
    SDL_SetRenderDrawColor( renderer, color->r, color->g, color->b, color->a );
    SDL_RenderFillRect( renderer, &raw_rect );
};

void render_close()
{
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
};
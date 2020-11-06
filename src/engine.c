#include "engine.h"
#include "input.h"
#include <SDL2/SDL.h>

static input_t sdl_key_to_input( SDL_Keycode key );

int engine_init()
{
    if ( SDL_Init( SDL_INIT_EVENTS | SDL_INIT_VIDEO ) != 0 )
    {
        SDL_Log( "Unable to initialize SDL: %s", SDL_GetError() );
        return 1;
    }
    return 0;
};

void engine_close()
{
    SDL_Quit();
};

int engine_handle_events()
{
    int running = 1;
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
                    input_register_press( sdl_key_to_input( event.key.keysym.sym ) );
                }
            }
            break;

            case ( SDL_KEYUP ):
            {
                input_register_release( sdl_key_to_input( event.key.keysym.sym ) );
            }
            break;
        }
    }
    return running;
};

int engine_get_ticks()
{
    return ( int )( SDL_GetTicks() );
};

void engine_delay( int delay )
{
    SDL_Delay( delay );
};

static input_t sdl_key_to_input( SDL_Keycode key )
{
    switch ( key )
    {
        case ( SDLK_UP )   : { return INPUT_UP;    } break;
        case ( SDLK_RIGHT ): { return INPUT_RIGHT; } break;
        case ( SDLK_DOWN ) : { return INPUT_DOWN;  } break;
        case ( SDLK_LEFT ) : { return INPUT_LEFT;  } break;
        case ( SDLK_z )    : { return INPUT_JUMP;  } break;
        default            : { return INPUT_NULL;  } break;
    }
}
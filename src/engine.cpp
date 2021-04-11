#include "engine.hpp"
#include "input.hpp"
#include <SDL2/SDL.h>

namespace Engine
{
    bool init()
    {
        if ( SDL_Init( SDL_INIT_VIDEO ) != 0 )
        {
            SDL_Log( "Unable to initialize SDL: %s", SDL_GetError() );
            return false;
        }
        Input::initKeys
        ({
            { Input::Key::RIGHT, { ( int )( SDLK_RIGHT ) } },
            { Input::Key::LEFT, { ( int )( SDLK_LEFT ) } }
        });
        return true;
    };

    void close()
    {
        SDL_Quit();
    };

    int getTicks()
    {
        return ( int )( SDL_GetTicks() );
    };

    bool handleEvents()
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
                        Input::press( ( int )( event.key.keysym.sym ) );
                    }
                }
                break;

                case ( SDL_KEYUP ):
                {
                    Input::release( ( int )( event.key.keysym.sym ) );
                }
                break;
            }
        }
        return running;
    };
}
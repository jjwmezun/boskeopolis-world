#include "input.h"

int inputs_held[ INPUT_MAX ];

void input_register( SDL_Keycode key, int value );

int input_held( input_t input )
{
    return inputs_held[ input ];
};

void input_register_press( SDL_Keycode key )
{
    input_register( key, 1 );
};

void input_register_release( SDL_Keycode key )
{
    input_register( key, 0 );
};

void input_init()
{
    for ( int i = 0; i < INPUT_MAX; ++i )
    {
        inputs_held[ i ] = 0;
    }
};

void input_register( SDL_Keycode key, int value )
{
    switch ( key )
    {
        case ( SDLK_UP ): { inputs_held[ INPUT_UP ] = value; } break;
        case ( SDLK_RIGHT ): { inputs_held[ INPUT_RIGHT ] = value; } break;
        case ( SDLK_DOWN ): { inputs_held[ INPUT_DOWN ] = value; } break;
        case ( SDLK_LEFT ): { inputs_held[ INPUT_LEFT ] = value; } break;
        case ( SDLK_z ): { inputs_held[ INPUT_JUMP ] = value; } break;
    }
};
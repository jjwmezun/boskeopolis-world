#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

typedef enum input_t
{
    INPUT_UP,
    INPUT_RIGHT,
    INPUT_DOWN,
    INPUT_LEFT,
    INPUT_JUMP,
    INPUT_MAX
} input_t;

int input_held( input_t input );
void input_register_press( SDL_Keycode key );
void input_register_release( SDL_Keycode key );
void input_init();

#endif
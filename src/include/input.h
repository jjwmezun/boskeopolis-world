#ifndef INPUT_H
#define INPUT_H

#include "vector.h"

enum InputKey
{
    INPUT_RIGHT,
    INPUT_LEFT,
    INPUT_NULL
};

#define INPUT_MAX INPUT_NULL

void input_init( Vector * new_keys );
void input_close();
void input_press( int raw_key );
void input_release( int raw_key );
void input_update();
int input_held_right();
int input_held_left();
int input_pressed_right();
int input_pressed_left();

#endif // INPUT_H
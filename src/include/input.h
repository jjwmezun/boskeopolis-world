#ifndef INPUT_H
#define INPUT_H

#include "vector.h"

enum InputKey
{
    INPUT_CONFIRM,
    INPUT_CANCEL,
    INPUT_MENU,
    INPUT_JUMP,
    INPUT_UP,
    INPUT_RIGHT,
    INPUT_DOWN,
    INPUT_LEFT,
    INPUT_NULL
};

#define INPUT_MAX INPUT_NULL

void input_init( Vector * new_keys );
void input_close();
void input_press( int raw_key );
void input_release( int raw_key );
void input_update();
int input_held_confirm();
int input_held_cancel();
int input_held_jump();
int input_held_up();
int input_held_right();
int input_held_down();
int input_held_left();
int input_pressed_confirm();
int input_pressed_cancel();
int input_pressed_menu();
int input_pressed_jump();
int input_pressed_up();
int input_pressed_right();
int input_pressed_down();
int input_pressed_left();

#endif // INPUT_H
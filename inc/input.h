#ifndef INPUT_H
#define INPUT_H

typedef enum input_t
{
    INPUT_UP,
    INPUT_RIGHT,
    INPUT_DOWN,
    INPUT_LEFT,
    INPUT_JUMP,
    INPUT_NULL
} input_t;

int input_held( input_t input );
void input_register_press( input_t input );
void input_register_release( input_t input );
void input_init();

#endif
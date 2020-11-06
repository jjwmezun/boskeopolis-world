#include "input.h"

int inputs_held[ INPUT_NULL ];

static void input_register( input_t input, int value );

int input_held( input_t input )
{
    return inputs_held[ input ];
};

void input_register_press( input_t input )
{
    input_register( input, 1 );
};

void input_register_release( input_t input )
{
    input_register( input, 0 );
};

void input_init()
{
    for ( int i = 0; i < INPUT_NULL; ++i )
    {
        inputs_held[ i ] = 0;
    }
};

static void input_register( input_t input, int value )
{
    if ( input != INPUT_NULL )
    {
        inputs_held[ input ] = value;
    }
};
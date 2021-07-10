#include "input.h"
#include "int_map.h"
#include <stdlib.h>

IntMap key_map;
int held[ INPUT_MAX ];
int pressed[ INPUT_MAX ];
int press_locks[ INPUT_MAX ];

void input_init( Vector * new_keys )
{
    // Init values.
    key_map = int_map_create( -1 );
    for ( int i = 0; i < INPUT_MAX; ++i )
    {
        held[ i ] = 0;
        pressed[ i ] = 0;
        press_locks[ i ] = 0;
    }

    // For each input, loop through keys for that value & set in map.
    for ( int i = 0; i < INPUT_MAX; ++i )
    {
        for ( int j = 0; j < new_keys[ i ].count; ++j )
        {
            int raw_key = new_keys[ i ].list[ j ].value.int_;
            Value v = int_map_get( &key_map, raw_key );
            Vector * l;
            // If key list doesn’t exist for this input, create 1.
            if ( v.type == VALUE_NULL )
            {
                l = ( Vector * )( calloc( 1, sizeof( Vector ) ) );
                int_map_add( &key_map, raw_key, value_create_unique_ptr( ( void * )( l ) ) );
            }
            else // Else just take the 1 that already exists.
            {
                l = ( Vector * )( v.value.ptr_ );
            }
            vector_push( l, value_create_int( i ) );
        }
    }
};

void input_close()
{
    for ( int i = 0; i < key_map.capacity; ++i )
    {
        if ( key_map.entries[ i ].key.number != -1 )
        {
            vector_destroy( ( Vector * )( key_map.entries[ i ].value.value.ptr_ ) );
        }
    }
    int_map_destroy( &key_map );
}

void input_press( int raw_key )
{
    Value v = int_map_get( &key_map, raw_key );
    if ( v.type != VALUE_NULL )
    {
        Vector * l = ( Vector * )( v.value.ptr_ );
        for ( int i = 0; i < l->count; ++i )
        {
            int key_num = l->list[ i ].value.int_;
            held[ key_num ] = 1;
            if ( !press_locks[ key_num ] )
            {
                pressed[ key_num ] = press_locks[ key_num ] = 1;
            }
        }
    }
};

void input_release( int raw_key )
{
    Value v = int_map_get( &key_map, raw_key );
    if ( v.type != VALUE_NULL )
    {
        Vector * l = ( Vector * )( v.value.ptr_ );
        for ( int i = 0; i < l->count; ++i )
        {
            int key_num = l->list[ i ].value.int_;
            held[ key_num ] = press_locks[ key_num ] = 0;
        }
    }
};

void input_update()
{
    for ( int i = 0; i < INPUT_MAX; ++i )
    {
        pressed[ i ] = 0;
    }
};

int input_held_confirm()
{
    return held[ INPUT_CONFIRM ];
};

int input_held_cancel()
{
    return held[ INPUT_CANCEL ];
};

int input_held_jump()
{
    return held[ INPUT_JUMP ];
};

int input_held_up()
{
    return held[ INPUT_UP ];
};

int input_held_right()
{
    return held[ INPUT_RIGHT ];
};

int input_held_down()
{
    return held[ INPUT_DOWN ];
};

int input_held_left()
{
    return held[ INPUT_LEFT ];
};

int input_pressed_confirm()
{
    return pressed[ INPUT_CONFIRM ];
};

int input_pressed_cancel()
{
    return pressed[ INPUT_CANCEL ];
};

int input_pressed_menu()
{
    return pressed[ INPUT_MENU ];
};

int input_pressed_jump()
{
    return pressed[ INPUT_JUMP ];
};

int input_pressed_up()
{
    return pressed[ INPUT_UP ];
};

int input_pressed_right()
{
    return pressed[ INPUT_RIGHT ];
};

int input_pressed_down()
{
    return pressed[ INPUT_DOWN ];
};

int input_pressed_left()
{
    return pressed[ INPUT_LEFT ];
};
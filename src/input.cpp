#include "input.hpp"
#include "int_map.hpp"
#include <stdlib.h>

IntMap key_map;
int held[ INPUT_MAX ];
int pressed[ INPUT_MAX ];
int press_locks[ INPUT_MAX ];

void input_init( Vector * new_keys )
{
    key_map = int_map_create( -1 );
    for ( int i = 0; i < INPUT_MAX; ++i )
    {
        held[ i ] = 0;
        pressed[ i ] = 0;
        press_locks[ i ] = 0;
    }

    for ( int i = 0; i < INPUT_MAX; ++i )
    {
        for ( int j = 0; j < new_keys[ i ].count; ++j )
        {
            int raw_key = new_keys[ i ].list[ j ].value.int_;
            Value v = int_map_get( &key_map, raw_key );
            Vector * l;
            if ( v.type == VALUE_NULL )
            {
                l = ( Vector * )( calloc( 1, sizeof( Vector ) ) );
                int_map_add( &key_map, raw_key, value_create_unique_ptr( ( void * )( l ) ) );
            }
            else
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

int input_held_right()
{
    return held[ ( int )( INPUT_RIGHT ) ];
};

int input_held_left()
{
    return held[ ( int )( INPUT_LEFT ) ];
};

int input_pressed_right()
{
    return pressed[ ( int )( INPUT_RIGHT ) ];
};

int input_pressed_left()
{
    return pressed[ ( int )( INPUT_LEFT ) ];
};
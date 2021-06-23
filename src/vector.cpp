#include <stdlib.h>
#include "vector.hpp"

#define VECTOR_DEFAULT_CAPACITY 32

Vector vector_create( int capacity )
{
    if ( capacity < 0 )
    {
        capacity = VECTOR_DEFAULT_CAPACITY;
    }
    Vector v;
    v.capacity = capacity;
    v.count = 0;
    v.list = ( Value * )( malloc( capacity * sizeof( Value ) ) );
    return v;
};

void vector_destroy( Vector * vector )
{
    for ( int i = 0; i < vector->count; ++i )
    {
        if ( vector->list[ i ].type == VALUE_UNIQUE_PTR )
        {
            free( vector->list[ i ].value.ptr_ );
        }
    }
    free( vector->list );
};

void vector_push( Vector * vector, Value value )
{
    if ( vector->count + 1 >= vector->capacity )
    {
        vector->capacity = vector->count * 2;
        Value * new_list = ( Value * )( malloc( vector->capacity * sizeof( Value ) ) );
        for ( int i = 0; i < vector->count; ++i )
        {
            new_list[ i ] = vector->list[ i ];
        }
        free( vector->list );
        vector->list = new_list;
    }
    vector->list[ vector->count++ ] = value;
};
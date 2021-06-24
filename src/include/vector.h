#ifndef VECTOR_H
#define VECTOR_H

#include "value.h"

typedef struct
{
    int count;
    int capacity;
    Value * list;
}
Vector;

Vector vector_create( int capacity );
void vector_destroy( Vector * vector );
void vector_push( Vector * vector, Value value );

#endif // VECTOR_H
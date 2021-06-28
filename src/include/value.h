#ifndef VALUE_H
#define VALUE_H

#include "color.h"
#include "rect.h"

typedef enum
{
    VALUE_NULL = 0,
    VALUE_INT,
    VALUE_FLOAT,
    VALUE_DOUBLE,
    VALUE_WEAK_PTR,
    VALUE_UNIQUE_PTR
} ValueType;

typedef struct
{
    ValueType type;
    union
    {
        int int_;
        float float_;
        double double_;
        void * ptr_;
    }
    value;
}
Value;

Value value_create_int( int value );
Value value_create_float( float value );
Value value_create_weak_ptr( void * value );
Value value_create_unique_ptr( void * value );

#endif // VALUE_H
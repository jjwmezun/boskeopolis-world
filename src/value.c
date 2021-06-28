#include "value.h"

Value value_create_int( int value )
{
    Value v;
    v.type = VALUE_INT;
    v.value.int_ = value;
    return v;
};

Value value_create_float( float value )
{
    Value v;
    v.type = VALUE_FLOAT;
    v.value.float_ = value;
    return v;
};

Value value_create_weak_ptr( void * value )
{
    Value v;
    v.type = VALUE_WEAK_PTR;
    v.value.ptr_ = value;
    return v;
};

Value value_create_unique_ptr( void * value )
{
    Value v;
    v.type = VALUE_UNIQUE_PTR;
    v.value.ptr_ = value;
    return v;
};
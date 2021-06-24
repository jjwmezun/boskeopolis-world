#ifndef ASSOC_ARRAY_H
#define ASSOC_ARRAY_H

#include <stdint.h>
#include "value.h"

typedef uint32_t hash_t;

typedef struct { char * string; hash_t hash; } AssocArrayKey;

typedef struct { AssocArrayKey key; Value value; } AssocArrayEntry;

typedef struct
{
    int count;
    int capacity;
    AssocArrayEntry * entries;
}
AssocArray;

AssocArray assoc_array_create( int init_capacity );
void assoc_array_destroy( AssocArray * array );
Value assoc_array_get( const AssocArray * array, const char * key );
void assoc_array_add( AssocArray * array, const char * key, Value value );
void assoc_array_debug( const AssocArray * array );

#endif // ASSOC_ARRAY_H
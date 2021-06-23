#ifndef ASSOC_ARRAY_H
#define ASSOC_ARRAY_H

#include <stdint.h>
#include "value.hpp"

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

AssocArray assoc_array_create();
void assoc_array_destroy( AssocArray * array );
Value assoc_array_get( AssocArray * array, const char * key );
void assoc_array_add( AssocArray * array, const char * key, Value value );

#endif // ASSOC_ARRAY_H
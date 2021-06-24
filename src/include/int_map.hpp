#ifndef INT_MAP_H
#define INT_MAP_H

#include <stdint.h>
#include "value.hpp"

typedef uint32_t hash_t;

typedef struct { int number; hash_t hash; } IntMapKey;

typedef struct { IntMapKey key; Value value; } IntMapEntry;

typedef struct
{
    int count;
    int capacity;
    IntMapEntry * entries;
}
IntMap;

IntMap int_map_create( int init_capacity );
void int_map_destroy( IntMap * map );
Value int_map_get( const IntMap * map, int key );
void int_map_add( IntMap * map, int key, Value value );

#endif // INT_MAP_H
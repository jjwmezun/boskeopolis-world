#include "int_map.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_MAP_START_CAPACITY 8

static IntMapEntry * int_map_find_entry( const IntMapEntry * entries, int capacity, int needle_number, hash_t needle_hash );
static hash_t int_map_hash( int key, int capacity );

IntMap int_map_create( int init_capacity )
{
    IntMap a;
    a.count = 0;
    a.capacity = ( init_capacity < 0 ) ? INT_MAP_START_CAPACITY : init_capacity;
    a.entries = ( IntMapEntry * )( calloc( a.capacity, sizeof( IntMapEntry ) ) );

    for ( int i = 0; i < a.capacity; ++i )
    {
        a.entries[ i ].key.hash = -1;
        a.entries[ i ].key.number = -1;
    }

    if ( a.entries == NULL )
    {
        log_error( "Failed to create int map." );
    }

    return a;
};

void int_map_destroy( IntMap * map )
{
    for ( int i = 0; i < map->capacity; ++i )
    {
        if ( map->entries[ i ].key.number != -1 && map->entries[ i ].value.type == VALUE_UNIQUE_PTR)
        {
            free( map->entries[ i ].value.value.ptr_ );
        }
    }
    free( map->entries );
};

Value int_map_get( const IntMap * map, int key )
{
    hash_t key_hash = int_map_hash( key, map->capacity );
    IntMapEntry * entry = int_map_find_entry( map->entries, map->capacity, key, key_hash );
    return entry->value;
};

void int_map_add( IntMap * map, int key, Value value )
{
    hash_t key_hash = int_map_hash( key, map->capacity );
    IntMapEntry * entry = int_map_find_entry( map->entries, map->capacity, key, key_hash );
    if ( entry->key.number == -1 )
    {
        if ( ( double )( map->count + 1 ) > ( double )( map->capacity ) * 0.75 )
        {
            int new_capacity = map->capacity * 2;
            IntMapEntry * new_list = ( IntMapEntry * )( calloc( new_capacity, sizeof( IntMapEntry ) ) );

            if ( new_list == NULL )
            {
                log_error( "Failed to grow int map." );
            }

            for ( int i = 0; i < new_capacity; ++i )
            {
                new_list[ i ].key.hash = -1;
                new_list[ i ].key.number = -1;
            }

            for ( int i = 0; i < map->capacity; ++i )
            {
                if ( map->entries[ i ].key.number != -1 )
                {
                    hash_t new_hash = int_map_hash( map->entries[ i ].key.number, new_capacity );
                    IntMapEntry * new_entry = int_map_find_entry( new_list, new_capacity, map->entries[ i ].key.number, new_hash );
                    new_entry->key.number = map->entries[ i ].key.number;
                    new_entry->key.hash = new_hash;
                    new_entry->value = map->entries[ i ].value;
                }
            }
            free( map->entries );
            map->entries = new_list;
            map->capacity = new_capacity;

            entry->key.hash = int_map_hash( key, map->capacity );
        }
        map->entries[ entry->key.hash ].key.hash = entry->key.hash;
        map->entries[ entry->key.hash ].key.number = key;
        map->entries[ entry->key.hash ].value = value;
        ++map->count;
    }
    else
    {
        entry->value = value;
    }
};

static IntMapEntry * int_map_find_entry( const IntMapEntry * entries, int capacity, int needle_number, hash_t needle_hash )
{
    while ( 1 )
    {
        IntMapEntry * entry = &entries[ needle_hash ];
        if ( entry->key.number == -1 || entry->key.number == needle_number )
        {
            entry->key.hash = needle_hash;
            return entry;
        }
        needle_hash = ( needle_hash + 1 ) % capacity;
    }
}

static hash_t int_map_hash( int key, int capacity )
{
    hash_t hash = 2166136261u;
    hash ^= ( uint8_t )( key );
    hash *= 16777619;
    return hash % capacity;
}
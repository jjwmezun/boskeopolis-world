#include <assert.h>
#include "assoc_array.hpp"
#include "log.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSOC_ARRAY_START_CAPACITY 8

static AssocArrayEntry * assoc_array_find_entry( const AssocArray * array, const char * needle_string, hash_t needle_hash );
static hash_t assoc_array_hash( const char * key, int capacity );

AssocArray assoc_array_create( int init_capacity )
{
    AssocArray a;
    a.count = 0;
    a.capacity = ( init_capacity < 0 ) ? ASSOC_ARRAY_START_CAPACITY : init_capacity;
    a.entries = ( AssocArrayEntry * )( calloc( a.capacity, sizeof( AssocArrayEntry ) ) );

    if ( a.entries == NULL )
    {
        log_error( "Failed to create associative array." );
    }

    /*
    for ( int i = 0; i < a.capacity; ++i )
    {
        a.entries[ i ].key.string = NULL;
        a.entries[ i ].key.hash = 0;
    }*/

    return a;
};

void assoc_array_destroy( AssocArray * array )
{
    for ( int i = 0; i < array->capacity; ++i )
    {
        if ( array->entries[ i ].key.string != NULL )
        {
            free( array->entries[ i ].key.string );
            if ( array->entries[ i ].value.type == VALUE_UNIQUE_PTR )
            {
                free( array->entries[ i ].value.value.ptr_ );
            }
        }
    }
    free( array->entries );
};

Value assoc_array_get( const AssocArray * array, const char * key )
{
    hash_t key_hash = assoc_array_hash( key, array->capacity );
    AssocArrayEntry * entry = assoc_array_find_entry( array, key, key_hash );
    return entry->value;
};

void assoc_array_add( AssocArray * array, const char * key, Value value )
{
    hash_t key_hash = assoc_array_hash( key, array->capacity );
    AssocArrayEntry * entry = assoc_array_find_entry( array, key, key_hash );
    if ( entry->key.string == NULL )
    {
        if ( array->count + 1 > array->capacity * 0.75 )
        {
            int new_capacity = array->capacity * 2;
            AssocArrayEntry * new_list = ( AssocArrayEntry * )( calloc( new_capacity, sizeof( AssocArrayEntry ) ) );

            if ( new_list == NULL )
            {
                log_error( "Failed to grow associative array." );
            }

            /*
            for ( int i = 0; i < new_capacity; ++i )
            {
                new_list[ i ].key.string = NULL;
                new_list[ i ].key.hash = NULL;
            }*/

            for ( int i = 0; i < array->capacity; ++i )
            {
                if ( array->entries[ i ].key.string != NULL )
                {
                    hash_t new_hash = assoc_array_hash( array->entries[ i ].key.string, new_capacity );
                    new_list[ new_hash ].key.string = array->entries[ i ].key.string;
                    new_list[ new_hash ].key.hash = new_hash;
                    new_list[ new_hash ].value = array->entries[ i ].value;
                }
            }
            free( array->entries );
            array->entries = new_list;
            array->capacity = new_capacity;

            entry->key.hash = assoc_array_hash( key, array->capacity );
        }
        array->entries[ entry->key.hash ].key.hash = entry->key.hash;
        array->entries[ entry->key.hash ].key.string = ( char * )( malloc( strlen( key ) + 1 ) );

        if ( array->entries[ entry->key.hash ].key.string == NULL )
        {
            log_error( "Failed to allocate string for associative array." );
        }

        strcpy( array->entries[ entry->key.hash ].key.string, key );
        array->entries[ entry->key.hash ].value = value;
        ++array->count;
    }
    else
    {
        entry->value = value;
    }
};

static AssocArrayEntry * assoc_array_find_entry( const AssocArray * array, const char * needle_string, hash_t needle_hash )
{
    while ( true )
    {
        AssocArrayEntry * entry = &array->entries[ needle_hash ];
        if ( entry->key.string == NULL || strcmp( entry->key.string, needle_string ) == 0 )
        {
            entry->key.hash = needle_hash;
            return entry;
        }
        needle_hash = ( needle_hash + 1 ) % array->capacity;
    }
}

static hash_t assoc_array_hash( const char * key, int capacity )
{
    hash_t hash = 2166136261u;
    const int length = strlen( key );
    for ( int i = 0; i < length; i++ )
    {
        hash ^= ( uint8_t )( key[ i ] );
        hash *= 16777619;
    }
    return hash % capacity;
}

void assoc_array_debug( const AssocArray * array )
{
    printf( "ASSOC ARRAY\n===================\n" );
    for ( int i = 0; i < array->capacity; ++i )
    {
        printf( "%d : ", i );
        if ( array->entries[ i ].key.string == NULL )
        {
            printf( "NULL\n" );
        }
        else
        {
            printf( "%s\n", array->entries[ i ].key.string );
        }
    }
    printf( "===================\n" );
};
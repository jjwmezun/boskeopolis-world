#include "filename.h"
#include "io.h"
#include "json/json.h"
#include "log.h"
#include "map.h"
#include "render.h"
#include <stdlib.h>
#include <string.h>
#include "unit.h"
#include "vector.h"

int map_create( Map * map, int state_number )
{
    map->w = 0;
    map->h = 0;
    map->num_o_collision_layers = 0;
    map->collision = NULL;

    Vector collision_layers = vector_create( -1 );

    char * source = filename_map( "blueberry-1" );
    char * text = io_read( source );
    free( source );
    if ( !text )
    {
        log_error( "Couldn’t load map file.\n" );
        return -1;
    }
    json_char * json = ( json_char * )( text );
    char error[ 8000 ];
    json_settings settings = { 0 };
    json_value * root = json_parse_ex( &settings, json, strlen( text ), error );
    free( text );
    if ( !root )
    {
        log_error( "Error parsing whole map json file." );
        return -1;
    }
    if ( root->type != json_object )
    {
        json_value_free( root );
        log_error( "Error parsing map json file.\n" );
        return -1;
    }
    for ( unsigned int i = 0; i < root->u.object.length; ++i )
    {
        const json_char * rootkey = root->u.object.values[ i ].name;
        if ( strcmp( rootkey, "width" ) == 0 )
        {
            json_value * obj = root->u.object.values[ i ].value;
            if ( obj->type != json_integer )
            {
                log_error( "Map json file isn’t formatted correctly.\n" );
                return -1;
            }
            map->w = obj->u.integer;
        }
        else if ( strcmp( rootkey, "height" ) == 0 )
        {
            json_value * obj = root->u.object.values[ i ].value;
            if ( obj->type != json_integer )
            {
                log_error( "Map json file isn’t formatted correctly.\n" );
                return -1;
            }
            map->h = obj->u.integer;
        }
        else if ( strcmp( rootkey, "backgroundcolor" ) == 0 )
        {
            json_value * obj = root->u.object.values[ i ].value;
            if ( obj->type != json_string )
            {
                log_error( "Map json file isn’t formatted correctly.\n" );
                return -1;
            }

            // Convert from hex string to 3 decimal digits.
            char colorstr[ 3 ][ 3 ];
            memcpy( colorstr[ 0 ], &obj->u.string.ptr[ 1 ], 2 ); // Start @ 1 to skip '#' symbol.
            memcpy( colorstr[ 1 ], &obj->u.string.ptr[ 3 ], 2 ); // Add by 2 digits for each.
            memcpy( colorstr[ 2 ], &obj->u.string.ptr[ 5 ], 2 );
            long int color[ 3 ];
            char * end;
            for ( int i = 0; i < 3; ++i )
            {
                color[ i ] = strtol( colorstr[ i ], &end, 16 );
                if ( end == colorstr[ i ] )
                {
                    log_error( "Map json file isn’t formatted correctly.\n" );
                    return -1;
                }
            }
            map->bgcolor.r = ( float )( color[ 0 ] );
            map->bgcolor.g = ( float )( color[ 1 ] );
            map->bgcolor.b = ( float )( color[ 2 ] );
            map->bgcolor.a = 255.0f;
        }
        else if ( strcmp( rootkey, "layers" ) == 0 )
        {
            json_value * obj = root->u.object.values[ i ].value;
            if ( obj->type != json_array )
            {
                log_error( "Map json file isn’t formatted correctly.\n" );
                return -1;
            }
            for ( unsigned int li = 0; li < obj->u.array.length; ++li )
            {
                json_value * layer = obj->u.array.values[ li ];
                if ( layer->type != json_object )
                {
                    log_error( "Map json file isn’t formatted correctly.\n" );
                    return -1;
                }

                char * name = NULL;
                json_value * data;
                int width = 0;
                int height = 0;

                for ( unsigned int ki = 0; ki < layer->u.object.length; ++ki )
                {

                    char * key = layer->u.object.values[ ki ].name;
                    json_value * value = layer->u.object.values[ ki ].value;

                    if ( strcmp( key, "data" ) == 0 )
                    {
                        if ( value->type != json_array )
                        {
                            log_error( "Map json file isn’t formatted correctly.\n" );
                            return -1;
                        }
                        data = value;
                    }
                    else if ( strcmp( key, "name" ) == 0 )
                    {
                        if ( value->type != json_string )
                        {
                            log_error( "Map json file isn’t formatted correctly.\n" );
                            return -1;
                        }
                        name = value->u.string.ptr;
                    }
                    else if ( strcmp( key, "width" ) == 0 )
                    {
                        if ( value->type != json_integer )
                        {
                            log_error( "Map json file isn’t formatted correctly.\n" );
                            return -1;
                        }
                        width = value->u.integer;
                    }
                    else if ( strcmp( key, "height" ) == 0 )
                    {
                        if ( value->type != json_integer )
                        {
                            log_error( "Map json file isn’t formatted correctly.\n" );
                            return -1;
                        }
                        height = value->u.integer;
                    }
                }

                if ( strcmp( name, "collision" ) == 0 )
                {
                    AssocArray array = assoc_array_create( -1 );
                    assoc_array_add( &array, "w", value_create_int( width ) );
                    assoc_array_add( &array, "h", value_create_int( height ) );
                    int * tiles = calloc( data->u.array.length, sizeof( int ) );
                    for ( unsigned int ti = 0; ti < data->u.array.length; ++ti )
                    {
                        json_value * tile = data->u.array.values[ ti ];
                        if ( tile->type != json_integer )
                        {
                            log_error( "Map json file isn’t formatted correctly.\n" );
                            return -1;
                        }
                        tiles[ ti ] = tile->u.integer;
                    }
                    assoc_array_add( &array, "data", value_create_unique_ptr( tiles ) );
                    vector_push( &collision_layers, value_create_weak_ptr( &array ) );
                }
            }
        }
    }
    json_value_free( root );

    map->num_o_collision_layers = collision_layers.count;
    map->collision = calloc( map->num_o_collision_layers, sizeof( int * ) );
    for ( int i = 0; i < collision_layers.count; ++i )
    {
        AssocArray * array = collision_layers.list[ i ].value.ptr_;
        if
        (
            map->w != assoc_array_get( array, "w" ).value.int_ ||
            map->h != assoc_array_get( array, "h" ).value.int_
        )
        {
            log_error( "Map json file isn’t formatted correctly.\n" );
            return -1;
        }
        map->collision[ i ] = calloc( map->w * map->h, sizeof( int ) );
        int * tiles = assoc_array_get( array, "data" ).value.ptr_;
        memcpy( map->collision[ i ], tiles, sizeof( int ) * map->w * map->h );
        assoc_array_destroy( array );
    }
    vector_destroy( &collision_layers );

    Graphic bg_graphic = graphic_create_full_rect( map->bgcolor );
    bg_graphic.abs = 1;
    render_add_graphic( bg_graphic, state_number, LAYER_BG_1 );
    for ( int i = 0; i < map->num_o_collision_layers; ++i )
    {
        render_add_graphic( graphic_create_tilemap( map->collision[ i ], map->w, map->h ), state_number, LAYER_BEFORE_BLOCKS_1 );
    }

    return 0;
};

void map_destroy( Map * map )
{
    for ( int i = 0; i < map->num_o_collision_layers; ++i )
    {
        free( map->collision[ i ] );
    }
    if ( map->collision != NULL )
    {
        free( map->collision );
    }
};
#include "asset.h"
#include <SDL2/SDL.h>
#include <string.h>

static char * graphics_path;
static char * map_path;
static char * localization_path;
static int graphics_path_length = 0;
static int map_path_length = 0;
static int localization_path_length = 0;

static int create_path( char ** dest, const char * base_path, int base_length, const char * local_path );
static char * get_path( const char * base_path, int base_path_length, const char * local );

void asset_init()
{
    // Initialize assets directory.
    char * base_path = SDL_GetBasePath();
    const int base_length = strlen( base_path ) + 1;
    graphics_path_length = create_path( &graphics_path, base_path, base_length, "assets/graphics/" );
    map_path_length = create_path( &map_path, base_path, base_length, "assets/maps/" );
    localization_path_length = create_path( &localization_path, base_path, base_length, "assets/localization/" );
    SDL_free( base_path );
};

void asset_close()
{
    free( graphics_path );
}

char * asset_localization( const char * local )
{
    return get_path( localization_path, localization_path_length, local );
};

char * asset_map( const char * local )
{
    return get_path( map_path, map_path_length, local );
};

char * asset_image( const char * local )
{
    return get_path( graphics_path, graphics_path_length, local );
};

char * asset_read_file_text( const char * filename )
{
    FILE * file = fopen( filename, "rb" );
    long length = 0;
    char * text = NULL;
    if ( file )
    {
        fseek( file, 0, SEEK_END );
        length = ftell( file );
        fseek( file, 0, SEEK_SET );
        text = calloc( sizeof( char ), length );
        if ( text )
        {
            fread( text, 1, length, file );
        }
        fclose ( file );
    }
    return text;
}

static int create_path( char ** dest, const char * base_path, int base_length, const char * local_path )
{
    const int full_length = base_length + strlen( local_path );
    *dest = calloc( full_length, sizeof( char ) );
    memcpy( *dest, base_path, base_length );
    strncat( *dest, local_path, full_length );
    return full_length;
};

static char * get_path( const char * base_path, int base_path_length, const char * local )
{
    const int full_filename_length = base_path_length + strlen( local );
    char * full_filename = calloc( full_filename_length, sizeof( char ) );
    memcpy( full_filename, base_path, base_path_length );
    strncat( full_filename, local, full_filename_length );
    return full_filename;
};
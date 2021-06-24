#include "filename.h"
#include "jstring.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #define PATH_DIVIDER "\\"
#else
    #define PATH_DIVIDER "/"
#endif

#define PATH_DIVIDER_SIZE strlen( PATH_DIVIDER )
#define SHADER_EXT ".glsl"
#define SHADER_EXT_SIZE strlen( SHADER_EXT )
#define JSON_EXT ".json"
#define JSON_EXT_SIZE strlen( JSON_EXT )

static char * master_directory = NULL;
static char * image_directory = NULL;
static char * shader_directory = NULL;
static char * localization_directory = NULL;
static int master_directory_size;
static int image_directory_size;
static int shader_directory_size;
static int localization_directory_size;

void filename_init()
{
    master_directory_size = strlen( "assets" ) + PATH_DIVIDER_SIZE + 1;
    master_directory = ( char * )( malloc( master_directory_size ) );
    if ( !master_directory )
    {
        log_error( "Could not allocate file directories for some reason.\n" );
        exit( -1 );
    }
    strcpy( master_directory, "assets" );
    strcat( master_directory, PATH_DIVIDER );

    // Don’t add extra 1 for NULL, since master_directory_size already has extra NULL.
    image_directory_size = master_directory_size + strlen( "graphics" ) + PATH_DIVIDER_SIZE;
    image_directory = ( char * )( malloc( image_directory_size ) );
    strcpy( image_directory, master_directory );
    strcat( image_directory, "graphics" );
    strcat( image_directory, PATH_DIVIDER );

    // Don’t add extra 1 for NULL, since master_directory_size already has extra NULL.
    localization_directory_size = master_directory_size + strlen( "localization" ) + PATH_DIVIDER_SIZE;
    localization_directory = ( char * )( malloc( localization_directory_size ) );
    strcpy( localization_directory, master_directory );
    strcat( localization_directory, "localization" );
    strcat( localization_directory, PATH_DIVIDER );

    // Don’t add extra 1 for NULL, since master_directory_size already has extra NULL.
    shader_directory_size = master_directory_size + strlen( "shaders" ) + PATH_DIVIDER_SIZE;
    shader_directory = ( char * )( malloc( shader_directory_size ) );
    strcpy( shader_directory, master_directory );
    strcat( shader_directory, "shaders" );
    strcat( shader_directory, PATH_DIVIDER );
};

void filename_close()
{
    free( master_directory );
    free( image_directory );
    free( localization_directory );
    free( shader_directory );
}

char * filename_image( const char * local )
{
    return jstring_concat( image_directory, local );
};

char * filename_localization( const char * local )
{
    const int size = localization_directory_size + strlen( local ) + JSON_EXT_SIZE;
    char * full_filename = ( char * )( malloc( size ) );
    strcpy( full_filename, localization_directory );
    strcat( full_filename, local );
    strcat( full_filename, JSON_EXT );
    return full_filename;
};

char * filename_shader( const char * local )
{
    const int size = shader_directory_size + strlen( local ) + SHADER_EXT_SIZE;
    char * full_filename = ( char * )( malloc( size ) );
    strcpy( full_filename, shader_directory );
    strcat( full_filename, local );
    strcat( full_filename, SHADER_EXT );
    return full_filename;
};
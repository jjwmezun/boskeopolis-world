#include <cstdio>
#include <cstdlib>
#include "io.hpp"

char * io_read( const char * source )
{
    // Open file.
    FILE * file = fopen( source, "rb" );

    if ( file == NULL )
    {
        return NULL;
    }

    // Go thru file to find size.
    fseek( file, 0L, SEEK_END );
    const size_t size = ftell( file );

    // Go back to start o’ file.
    rewind( file );

    // Allocate buffer based on found size.
    char * buffer = ( char * )( malloc( size + 1 ) );

    if ( buffer == NULL )
    {
        return NULL;
    }

    // Read file into buffer & get buffer length.
    const size_t bytes = fread( buffer, sizeof( char ), size, file );

    if ( bytes < size )
    {
        return NULL;
    }

    // Make sure buffer ends with string terminator.
    buffer[ bytes ] = '\0';

    fclose( file );
    return buffer;
};
#include <cstdio>
#include <cstdlib>
#include "io.hpp"
#include "log.hpp"

namespace IO
{
    std::string read( const char * source )
    {
        // Open file.
        FILE * file = fopen( source, "rb" );

        if ( file == NULL )
        {
            Log::sendError( "IO: Failed to open file “" + std::string( source ) + "”" );
            exit( -1 );
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
            Log::sendError( "IO: Failed to allocate buffer for file “" + std::string( source ) + "”" );
            exit( -1 );
        }

        // Read file into buffer & get buffer length.
        const size_t bytes = fread( buffer, sizeof( char ), size, file );

        if ( bytes < size )
        {
            Log::sendError( "IO: Buffer size wrong for “" + std::string( source ) + "”" );
            exit( -1 );
        }

        // Make sure buffer ends with string terminator.
        buffer[ bytes ] = '\0';

        fclose( file );
        std::string s { buffer };
        free( buffer );
        return s;
    };

    std::string read( const std::string & source )
    {
        return read( source.c_str() );
    };
}
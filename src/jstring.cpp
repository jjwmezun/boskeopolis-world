#include "jstring.hpp"
#include <stdlib.h>
#include <string.h>

char * jstring_concat( const char * a, const char * b )
{
    const int size = strlen( a ) + strlen( b ) + 1;
    char * out = ( char * )( malloc( size ) );
    strcpy( out, a );
    strcat( out, b );
    return out;
};
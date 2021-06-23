#include <stdio.h>
#include "log.hpp"

void log_error( const char * string )
{
    printf( "ERROR: %s\n", string );
};
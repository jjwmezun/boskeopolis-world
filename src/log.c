#include <stdio.h>
#include "log.h"

void log_error( const char * string )
{
    printf( "ERROR: %s\n", string );
};
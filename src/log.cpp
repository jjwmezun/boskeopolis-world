#include <stdio.h>
#include "log.h"

char * latest_error = "No Error";

void log_error( char * string )
{
    latest_error = string;
    printf( "ERROR: %s\n", string );
};

const char * log_get_error()
{
    return latest_error;
};
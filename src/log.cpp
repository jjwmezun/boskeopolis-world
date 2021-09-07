#include "config.hpp"
#include <cstdio>
#include "log.hpp"
#include <vector>

std::vector<std::string> errors;

namespace Log
{
    void sendError( std::string string )
    {
        string += "\n";
        #ifdef DEBUG
            printf( "ERROR: %s\n", string.c_str() );
        #endif
        errors.emplace_back( std::move( string ) );
    };
}
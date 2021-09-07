#include <cstdio>
#include "log.hpp"
#include <vector>

std::vector<std::string> errors;

namespace Log
{
    void sendError( std::string string )
    {
        string += "\n";
        printf( "ERROR: %s\n", string.c_str() );
        errors.emplace_back( std::move( string ) );
    };
}
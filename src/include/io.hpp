#ifndef IO_H
#define IO_H

#include <string>

namespace IO
{
    std::string read( const char * source );
    std::string read( const std::string & source );
}

#endif
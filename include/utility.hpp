#ifndef UTILITY_H
#define UTILITY_H

#include <stdexcept>
#include <string>
#include <unordered_map>

namespace BSW
{
    template<typename T>
    T findInMap( std::unordered_map<std::string, T> m, std::string key, std::string error )
    {
        auto it = m.find( key );
        if ( it == m.end() )
        {
            throw std::runtime_error( error );
        }
        return it->second;
    };

    std::string strReplace( const std::string & haystack, const std::string & needle, const std::string & replace );
}

#endif // UTILITY_H
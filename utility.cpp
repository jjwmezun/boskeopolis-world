#include <regex>
#include "utility.hpp"

namespace BSW
{
    std::string strReplace( const std::string & haystack, const std::string & needle, const std::string & replace )
    {
        std::regex reg = std::regex( needle );
        return std::regex_replace( haystack, reg, replace );
    };
}
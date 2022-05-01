#include <algorithm>
#include "string.hpp"

namespace String
{
    std::string pad( const std::string & text, std::size_t padding )
    {
        // Courtest o’ https://stackoverflow.com/questions/6143824/add-leading-zeros-to-string-without-sprintf
        return std::string( padding - std::min( padding, text.length() ), '0' ) + text;
    };
};
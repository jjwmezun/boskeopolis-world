#include "text.hpp"

Text::Text( const char * text, std::unordered_map<const char *, int> args )
:
    lines_ ( { text } )
{};

void Text::render() const
{
    for ( auto & line : lines_ )
    {
        printf( "%s\n", line.c_str() );
    }
};
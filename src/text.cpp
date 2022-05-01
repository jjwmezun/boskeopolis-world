#include "localization.hpp"
#include "render.hpp"
#include "text.hpp"
#include "unit.hpp"

#define MAX_CHARACTER_TYPES 1009 // Note: must be prime.
#define MAX_TEXT_LINES 32
#define MAX_CHARACTERS_PER_LINE 256

static int getCharacterSize( const char * s );

Text::Text( const char * text, std::unordered_map<const char *, std::variant<float, Align, VAlign, Color>> args )
{
    for ( auto & i : args )
    {
        if ( std::string( i.first ) == "x" )
        {
            x_ = std::get<float>( i.second );
        }
        if ( std::string( i.first ) == "y" )
        {
            y_ = std::get<float>( i.second );
        }
        if ( std::string( i.first ) == "w" )
        {
            w_ = std::get<float>( i.second );
        }
        if ( std::string( i.first ) == "h" )
        {
            h_ = std::get<float>( i.second );
        }
        else if ( std::string( i.first ) == "align" )
        {
            align_ = std::get<Align>( i.second );
        }
        else if ( std::string( i.first ) == "valign" )
        {
            valign_ = std::get<VAlign>( i.second );
        }
        else if ( std::string( i.first ) == "x_padding" || std::string( i.first ) == "padding_x" )
        {
            padding_x_ = std::get<float>( i.second );
        }
        else if ( std::string( i.first ) == "y_padding" || std::string( i.first ) == "padding_y" )
        {
            padding_y_ = std::get<float>( i.second );
        }
        else if ( std::string( i.first ) == "color" )
        {
            color_ = std::get<Color>( i.second );
        }
    }

    setCharacters( text );
};

void Text::setCharacters( const char * text )
{
    characters_.clear();
    float w = w_ - ( padding_x_ * 2.0 );
    float h = h_ - ( padding_y_ * 2.0 );
    float x = x_ + padding_x_;
    float y = y_ + padding_y_;
    const float line_end = x + w;

    const auto & character_map = Localization::getCurrentLanguage().chars;

    std::vector<CharacterTemplate> char_list;

    // 1st loop gets list o’ characters with sizes.
    while ( *text )
    {
        std::string string = text;
        int character_size = getCharacterSize( text );
        std::string letter = string.substr( 0, character_size );

        const auto iterator = character_map.find( letter.c_str() );
        if ( iterator != character_map.end() )
        {
            char_list.push_back( iterator->second );
        }

        text += character_size;
    }

    // 2nd loop: break characters into lines, broken by newlines or when a line reaches the width limit.
    CharacterTemplate lines[ MAX_TEXT_LINES ][ MAX_CHARACTERS_PER_LINE ];
    float line_widths[ MAX_TEXT_LINES ];
    line_widths[ 0 ] = 0;
    int line_character_counts[ MAX_TEXT_LINES ];
    int line_count = 0;
    int line_character = 0;
    long unsigned int i = 0;
    int lx = ( int )( x );
    while ( i < char_list.size() )
    {
        long unsigned int ib = i;
        float xb = lx;
        int look_ahead = 1;

        // Look ahead so we can know ahead o’ time whether we need to add a new line.
        // This autobreaks text without cutting midword.
        while ( look_ahead )
        {
            if ( ib >= char_list.size() )
            {
                look_ahead = 0;
                break;
            }

            if ( char_list[ ib ].type == CharacterType::NEWLINE )
            {
                look_ahead = 0;
            }
            else if ( char_list[ ib ].type == CharacterType::WHITESPACE )
            {
                look_ahead = 0;
            }
            else if ( xb >= line_end )
            {
                lx = ( int )( x );
                line_character_counts[ line_count ] = line_character;
                ++line_count;
                line_widths[ line_count ] = 0;
                line_character = 0;
                look_ahead = 0;
            }
            else if ( ib >= char_list.size() )
            {
                look_ahead = 0;
                break;
            }
            xb += char_list[ ib ].w;
            ++ib;
        }

        while ( i < ib )
        {
            if ( char_list[ i ].type == CharacterType::NEWLINE || lx >= line_end )
            {
                lx = ( int )( x );
                line_character_counts[ line_count ] = line_character;
                ++line_count;
                line_widths[ line_count ] = 0;
                line_character = 0;
            }
            else
            {
                lines[ line_count ][ line_character ] = char_list[ i ];
                line_widths[ line_count ] += char_list[ i ].w;
            }
            ++i;
            ++line_character;
            lx += char_list[ i ].w;
        }
    }
    line_character_counts[ line_count ] = line_character;
    ++line_count;


    // Sometimes the previous loop keeps whitespace @ the end o’ lines.
    // Since this messes with x alignment, remove these.
    for ( int l = 0; l < line_count; ++l )
    {
        if ( lines[ l ][ line_character_counts[ l ] - 1 ].type == CharacterType::WHITESPACE )
        {
            --line_character_counts[ l ];
            line_widths[ l ] -= lines[ l ][ line_character_counts[ l ] - 1 ].w;
        }
    }

    // Final loop: we have all the info we need now to set x & y positions.
    float dy = ( valign_ == VAlign::MIDDLE )
        ? y + ( ( h - ( line_count * 8.0 ) ) / 2.0 )
        : ( valign_ == VAlign::BOTTOM )
            ? y + h - ( line_count * 8.0 )
            : y;
    for ( int l = 0; l < line_count; ++l )
    {
        float dx = ( align_ == Align::CENTER )
            ? x + ( ( w - line_widths[ l ] ) / 2.0 )
            : ( align_ == Align::RIGHT )
                ? line_end - line_widths[ l ]
                : x;
        for ( int c = 0; c < line_character_counts[ l ]; ++c )
        {
            // Just in case o’ character index misalignment, just copy o’er whole characters.
            characters_.push_back( { lines[ l ][ c ].w, lines[ l ][ c ].h, lines[ l ][ c ].x, lines[ l ][ c ].y, dx, dy, color_ } );
            dx += lines[ l ][ c ].w;
        }
        dy += 8.0;
    }
};

static int getCharacterSize( const char * s )
{
    const int code = ( int )( *s );
    return ( code & ( 1 << 7 ) )
        ? ( code & ( 1 << 5 ) )
            ? ( code & ( 1 << 4 ) )
                ? 4
                : 3
            : 2
        : 1;
};
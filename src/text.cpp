#include "localization.hpp"
#include "localization_language.hpp"
#include "render.hpp"
#include "text.hpp"
#include "unit.hpp"


#define MAX_CHARACTER_TYPES 1009 // Note: must be prime.
#define MAX_TEXT_LINES 32
#define MAX_CHARACTERS_PER_LINE 256

static int getCharacterSize( const char * s );

Text::~Text() {};

Text::Text( const char * text, std::unordered_map<const char *, std::variant<double, Align, VAlign, Color>> args )
{
    double x = 0;
    double y = 0;
    double w = Unit::WINDOW_WIDTH_PIXELS;
    double h = Unit::WINDOW_HEIGHT_PIXELS;
    Align align = Align::LEFT;
    VAlign valign = VAlign::TOP;
    double padding_x = 0;
    double padding_y = 0;
    Color color = { 0, 0, 0, 255 };

    for ( auto & i : args )
    {
        if ( std::string( i.first ) == "x" )
        {
            x = std::get<double>( i.second );
        }
        if ( std::string( i.first ) == "y" )
        {
            y = std::get<double>( i.second );
        }
        if ( std::string( i.first ) == "w" )
        {
            w = std::get<double>( i.second );
        }
        if ( std::string( i.first ) == "h" )
        {
            h = std::get<double>( i.second );
        }
        else if ( std::string( i.first ) == "align" )
        {
            align = std::get<Align>( i.second );
        }
        else if ( std::string( i.first ) == "valign" )
        {
            valign = std::get<VAlign>( i.second );
        }
        else if ( std::string( i.first ) == "x_padding" || std::string( i.first ) == "padding_x" )
        {
            padding_x = std::get<double>( i.second );
        }
        else if ( std::string( i.first ) == "y_padding" || std::string( i.first ) == "padding_y" )
        {
            padding_y = std::get<double>( i.second );
        }
        else if ( std::string( i.first ) == "color" )
        {
            color = std::get<Color>( i.second );
        }
    }

    w -= ( padding_x * 2.0 );
    h -= ( padding_y * 2.0 );
    x += padding_x;
    y += padding_y;
    const double line_end = x + w;

    const auto & character_map = Localization::getLanguage().getCharacterMap();

    // 1st loop gets list o’ characters with sizes.
    while ( *text )
    {
        std::string string = text;
        int character_size = getCharacterSize( text );
        std::string letter = string.substr( 0, character_size );

        const auto iterator = character_map.find( letter.c_str() );
        if ( iterator != character_map.end() )
        {
            Character character = iterator->second;
            character.color = color;
            characters_.push_back( character );
        }

        text += character_size;
    }

    // 2nd loop: break characters into lines, broken by newlines or when a line reaches the width limit.
    Character lines[ MAX_TEXT_LINES ][ MAX_CHARACTERS_PER_LINE ];
    double line_widths[ MAX_TEXT_LINES ];
    line_widths[ 0 ] = 0;
    int line_character_counts[ MAX_TEXT_LINES ];
    int line_count = 0;
    int line_character = 0;
    int i = 0;
    int lx = ( int )( x );
    while ( i < characters_.size() )
    {
        int ib = i;
        double xb = lx;
        int look_ahead = 1;

        // Look ahead so we can know ahead o’ time whether we need to add a new line.
        // This autobreaks text without cutting midword.
        while ( look_ahead )
        {
            if ( ib >= characters_.size() )
            {
                look_ahead = 0;
                break;
            }

            if ( characters_[ ib ].type == Character::Type::NEWLINE )
            {
                look_ahead = 0;
            }
            else if ( characters_[ ib ].type == Character::Type::WHITESPACE )
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
            else if ( ib >= characters_.size() )
            {
                look_ahead = 0;
                break;
            }
            xb += characters_[ ib ].src.w;
            ++ib;
        }

        while ( i < ib )
        {
            if ( characters_[ i ].type == Character::Type::NEWLINE || lx >= line_end )
            {
                lx = ( int )( x );
                line_character_counts[ line_count ] = line_character;
                ++line_count;
                line_widths[ line_count ] = 0;
                line_character = 0;
            }
            else
            {
                lines[ line_count ][ line_character ] = characters_[ i ];
                line_widths[ line_count ] += characters_[ i ].src.w;
            }
            ++i;
            ++line_character;
            lx += characters_[ i ].src.w;
        }
    }
    line_character_counts[ line_count ] = line_character;
    ++line_count;


    // Sometimes the previous loop keeps whitespace @ the end o’ lines.
    // Since this messes with x alignment, remove these.
    for ( int l = 0; l < line_count; ++l )
    {
        if ( lines[ l ][ line_character_counts[ l ] - 1 ].type == Character::Type::WHITESPACE )
        {
            --line_character_counts[ l ];
            line_widths[ l ] -= lines[ l ][ line_character_counts[ l ] - 1 ].src.w;
        }
    }

    // Final loop: we have all the info we need now to set x & y positions.
    characters_.clear();
    double dy = ( valign == VAlign::MIDDLE )
        ? y + ( ( h - ( line_count * 8.0 ) ) / 2.0 )
        : ( valign == VAlign::BOTTOM )
            ? y + h - ( line_count * 8.0 )
            : y;
    for ( int l = 0; l < line_count; ++l )
    {
        double dx = ( align == Align::CENTER )
            ? x + ( ( w - line_widths[ l ] ) / 2.0 )
            : ( align == Align::RIGHT )
                ? line_end - line_widths[ l ]
                : x;
        for ( int c = 0; c < line_character_counts[ l ]; ++c )
        {
            // Just in case o’ character index misalignment, just copy o’er whole characters.
            lines[ l ][ c ].dest.x = dx;
            lines[ l ][ c ].dest.y = dy;
            dx += lines[ l ][ c ].dest.w;
            characters_.push_back( lines[ l ][ c ] );
        }
        dy += 8.0;
    }
};

void Text::render() const
{
    for ( const auto & character : characters_ )
    {
        Render::character( character );
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
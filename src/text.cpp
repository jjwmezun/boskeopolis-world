#include "localization.hpp"
#include "localization_language.hpp"
#include "render.hpp"
#include "text.hpp"
#include "unit.hpp"


#define MAX_CHARACTER_TYPES 1009 // Note: must be prime.
#define MAX_TEXT_LINES 32
#define MAX_temp_charactersPER_LINE 256

static int getCharacterSize( const char * s );

Text Text::create( const char * text, std::unordered_map<const char *, std::variant<double, Align, VAlign, Color>> args )
{
    Text t;
    //t.characters_ = {};
    t.number_of_characters_ = 0;
    t.color_ = { 0.0, 0.0, 0.0, 255.0 };

    double x = 0;
    double y = 0;
    double w = Unit::WINDOW_WIDTH_PIXELS;
    double h = Unit::WINDOW_HEIGHT_PIXELS;
    Align align = Align::LEFT;
    VAlign valign = VAlign::TOP;
    double padding_x = 0;
    double padding_y = 0;

    // Set variables based on args.
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
            t.color_ = std::get<Color>( i.second );
        }
    }

    w -= ( padding_x * 2.0 );
    h -= ( padding_y * 2.0 );
    x += padding_x;
    y += padding_y;
    const double line_end = x + w;

    std::vector<CharacterTemplate> temp_characters;

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
            CharacterTemplate character = iterator->second;
            temp_characters.push_back( character );
        }

        text += character_size;
    }

    // 2nd loop: break characters into lines, broken by newlines or when a line reaches the width limit.
    CharacterTemplate lines[ MAX_TEXT_LINES ][ MAX_temp_charactersPER_LINE ];
    double line_widths[ MAX_TEXT_LINES ];
    line_widths[ 0 ] = 0;
    int line_character_counts[ MAX_TEXT_LINES ];
    int line_count = 0;
    int line_character = 0;
    int i = 0;
    int lx = ( int )( x );
    while ( i < temp_characters.size() )
    {
        int ib = i;
        double xb = lx;
        int look_ahead = 1;

        // Look ahead so we can know ahead o’ time whether we need to add a new line.
        // This autobreaks text without cutting midword.
        while ( look_ahead )
        {
            if ( ib >= temp_characters.size() )
            {
                look_ahead = 0;
                break;
            }

            if ( temp_characters[ ib ].type == CharacterTemplate::Type::NEWLINE )
            {
                look_ahead = 0;
            }
            else if ( temp_characters[ ib ].type == CharacterTemplate::Type::WHITESPACE )
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
            else if ( ib >= temp_characters.size() )
            {
                look_ahead = 0;
                break;
            }
            xb += temp_characters[ ib ].w;
            ++ib;
        }

        while ( i < ib )
        {
            if ( temp_characters[ i ].type == CharacterTemplate::Type::NEWLINE || lx >= line_end )
            {
                lx = ( int )( x );
                line_character_counts[ line_count ] = line_character;
                ++line_count;
                line_widths[ line_count ] = 0;
                line_character = 0;
            }
            else
            {
                lines[ line_count ][ line_character ] = temp_characters[ i ];
                line_widths[ line_count ] += temp_characters[ i ].w;
            }
            ++i;
            ++line_character;
            lx += temp_characters[ i ].w;
        }
    }
    line_character_counts[ line_count ] = line_character;
    ++line_count;


    // Sometimes the previous loop keeps whitespace @ the end o’ lines.
    // Since this messes with x alignment, remove these.
    for ( int l = 0; l < line_count; ++l )
    {
        if ( lines[ l ][ line_character_counts[ l ] - 1 ].type == CharacterTemplate::Type::WHITESPACE )
        {
            --line_character_counts[ l ];
            line_widths[ l ] -= lines[ l ][ line_character_counts[ l ] - 1 ].w;
        }
    }

    // Final loop: we have all the info we need now to set x & y positions.
    float dy = ( valign == VAlign::MIDDLE )
        ? y + ( ( h - ( line_count * 8.0 ) ) / 2.0 )
        : ( valign == VAlign::BOTTOM )
            ? y + h - ( line_count * 8.0 )
            : y;
    for ( int l = 0; l < line_count; ++l )
    {
        float dx = ( align == Align::CENTER )
            ? x + ( ( w - line_widths[ l ] ) / 2.0 )
            : ( align == Align::RIGHT )
                ? line_end - line_widths[ l ]
                : x;
        for ( int c = 0; c < line_character_counts[ l ]; ++c )
        {
            // Just in case o’ character index misalignment, just copy o’er whole characters.
            dx += lines[ l ][ c ].w;
            t.characters_[ t.number_of_characters_++ ] = 
            {
                lines[ l ][ c ].w,
                lines[ l ][ c ].h,
                lines[ l ][ c ].x,
                lines[ l ][ c ].y,
                dx,
                dy
            };
        }
        dy += 8.0;
    }
    return t;
};

void Text::render() const
{
    for ( int i = 0; i < number_of_characters_; ++i )
    {
        Render::character( characters_[ i ], color_ );
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
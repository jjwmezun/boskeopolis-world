#include "assoc_array.h"
#include "character_template.h"
#include "localization.h"
#include "render.h"
#include <stdlib.h>
#include <string.h>
#include "text.h"
#include "unit.h"
#include "vector.h"

#define MAX_CHARACTER_TYPES 1009 // Note: must be prime.
#define MAX_TEXT_LINES 32
#define MAX_CHARS_PER_LINE 256

static int text_get_character_size( const char * s );

Text text_create( const char * text, AssocArray * args )
{
    Text t;
    t.number_of_characters = 0;
    t.color.a = 255.0;

    double x = 0;
    double y = 0;
    double w = WINDOW_WIDTH_PIXELS;
    double h = WINDOW_HEIGHT_PIXELS;
    TextAlign align = ALIGN_LEFT;
    TextVAlign valign = VALIGN_TOP;
    double padding_x = 0;
    double padding_y = 0;

    // Set variables based on args.
    Value xentry = assoc_array_get( args, "x" );
    if ( xentry.type == VALUE_FLOAT )
    {
        x = xentry.value.float_;
    }

    Value yentry = assoc_array_get( args, "y" );
    if ( yentry.type == VALUE_FLOAT )
    {
        y = yentry.value.float_;
    }

    Value alignentry = assoc_array_get( args, "align" );
    if ( alignentry.type == VALUE_INT )
    {
        align = alignentry.value.int_;
    }

    Value valignentry = assoc_array_get( args, "valign" );
    if ( valignentry.type == VALUE_INT )
    {
        valign = valignentry.value.int_;
    }

    Value colorentry = assoc_array_get( args, "color" );
    if ( colorentry.type == VALUE_UNIQUE_PTR || colorentry.type == VALUE_WEAK_PTR )
    {
        Color * cp = colorentry.value.ptr_;
        t.color = *cp;
    }

    w -= ( padding_x * 2.0 );
    h -= ( padding_y * 2.0 );
    x += padding_x;
    y += padding_y;
    const double line_end = x + w;

    Vector tempchars = vector_create( MAX_CHARACTERS );

    const AssocArray * charmap = &localization_get_current()->charmap;

    // 1st loop gets list o’ characters with sizes.
    while ( *text )
    {
        int character_size = text_get_character_size( text );

        char * letter = ( char * )( malloc( character_size + 1 ) );
        int i = 0;
        while ( i < character_size )
        {
            letter[ i ] = text[ i ];
            ++i;
        }
        letter[ i ] = '\0';

        Value v = assoc_array_get( charmap, letter );

        if ( v.type == VALUE_UNIQUE_PTR )
        {
            void * c = malloc( sizeof( CharacterTemplate ) );
            memcpy( c, v.value.ptr_, sizeof( CharacterTemplate ) );
            vector_push( &tempchars, value_create_unique_ptr( c ) );
        }

        free( letter );

        text += character_size;
    }

    // 2nd loop: break characters into lines, broken by newlines or when a line reaches the width limit.
    CharacterTemplate lines[ MAX_TEXT_LINES ][ MAX_CHARS_PER_LINE ];
    double line_widths[ MAX_TEXT_LINES ];
    line_widths[ 0 ] = 0;
    int line_character_counts[ MAX_TEXT_LINES ];
    int line_count = 0;
    int line_character = 0;
    int i = 0;
    int lx = ( int )( x );
    while ( i < tempchars.count )
    {
        int ib = i;
        double xb = lx;
        int look_ahead = 1;

        // Look ahead so we can know ahead o’ time whether we need to add a new line.
        // This autobreaks text without cutting midword.
        while ( look_ahead )
        {
            if ( ib >= tempchars.count )
            {
                look_ahead = 0;
                break;
            }

            CharacterTemplate * ibv = ( ( CharacterTemplate * )( tempchars.list[ ib ].value.ptr_ ) );
            if ( ibv->type == CHAR_NEWLINE )
            {
                look_ahead = 0;
            }
            else if ( ibv->type == CHAR_WHITESPACE )
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
            else if ( ib >= tempchars.count )
            {
                look_ahead = 0;
                break;
            }
            xb += ibv->w;
            ++ib;
        }

        while ( i < ib )
        {
            if ( ( ( CharacterTemplate * )( tempchars.list[ i ].value.ptr_ ) )->type == CHAR_NEWLINE || lx >= line_end )
            {
                lx = ( int )( x );
                line_character_counts[ line_count ] = line_character;
                ++line_count;
                line_widths[ line_count ] = 0;
                line_character = 0;
            }
            else
            {
                memcpy( &lines[ line_count ][ line_character ], tempchars.list[ i ].value.ptr_, sizeof( CharacterTemplate ) );
                line_widths[ line_count ] += ( ( CharacterTemplate * )( tempchars.list[ i ].value.ptr_ ) )->w;
            }
            ++i;
            ++line_character;
            if ( i < tempchars.count )
            {
                lx += ( ( CharacterTemplate * )( tempchars.list[ i ].value.ptr_ ) )->w;
            }
        }
    }
    line_character_counts[ line_count ] = line_character;
    ++line_count;

    vector_destroy( &tempchars );

    // Sometimes the previous loop keeps whitespace @ the end o’ lines.
    // Since this messes with x alignment, remove these.
    for ( int l = 0; l < line_count; ++l )
    {
        if ( lines[ l ][ line_character_counts[ l ] - 1 ].type == CHAR_WHITESPACE )
        {
            --line_character_counts[ l ];
            line_widths[ l ] -= lines[ l ][ line_character_counts[ l ] - 1 ].w;
        }
    }

    // Final loop: we have all the info we need now to set x & y positions.
    float dy = ( valign == VALIGN_MIDDLE )
        ? y + ( ( h - ( line_count * 8.0 ) ) / 2.0 )
        : ( valign == VALIGN_BOTTOM )
            ? y + h - ( line_count * 8.0 )
            : y;
    for ( int l = 0; l < line_count; ++l )
    {
        float dx = ( align == ALIGN_CENTER )
            ? x + ( ( w - line_widths[ l ] ) / 2.0 )
            : ( align == ALIGN_RIGHT )
                ? line_end - line_widths[ l ]
                : x;
        for ( int c = 0; c < line_character_counts[ l ]; ++c )
        {
            // Just in case o’ character index misalignment, just copy o’er whole characters.
            dx += lines[ l ][ c ].w;
            t.characters[ t.number_of_characters ].w = lines[ l ][ c ].w;
            t.characters[ t.number_of_characters ].h = lines[ l ][ c ].h;
            t.characters[ t.number_of_characters ].src_x = lines[ l ][ c ].x;
            t.characters[ t.number_of_characters ].src_y = lines[ l ][ c ].y;
            t.characters[ t.number_of_characters ].dest_x = dx;
            t.characters[ t.number_of_characters ].dest_y = dy;
            ++t.number_of_characters;
        }
        dy += 8.0;
    }
    return t;
};

static int text_get_character_size( const char * s )
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
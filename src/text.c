#include "asset.h"
#include "config.h"
#include "json.h"
#include "text.h"

#define MAX_CHARACTER_TYPES 1009 // Note: must be prime.
#define MAX_TEXT_LINES 32
#define MAX_CHARACTERS_PER_LINE 256

typedef struct character_template_t
{
    int filled;
    char string[ 5 ];
    double x;
    double y;
    double w;
    character_type_t type;
} character_template_t;

character_template_t characters[ MAX_CHARACTER_TYPES ];

static get_character_size( const char * s );
static char * get_string_from_character( const character_t * c );
static int get_hash( const char * text, int character_size );

text_t text_create( const char * text, text_args_t args )
{
    text_t o;
    o.count = 0;
    args.w -= ( args.x_padding * 2.0 ); // Adjust width & height
    args.h -= ( args.y_padding * 2.0 ); // to leave room for padding.
    char string[ 5 ];
    args.x += args.x_padding;
    args.y += args.y_padding;
    double x = args.x;
    const double line_end = x + args.w;

    // 1st loop gets list o’ characters with sizes.
    while ( *text && o.count < 256 ) // Make sure we don’t o’erflow limit.
    {
        int character_size = get_character_size( text );
        int hash = get_hash( text, character_size );

        // Set substring so we can use it later.
        strncpy( string, text, character_size );
        string[ character_size ] = '\0';

        // Advance text.
        text += character_size;

        // If filled, but not equal, then there was a collision.
        // Since this uses open addressing, keep going forward till we find it.
        while ( characters[ hash ].filled && strcmp( characters[ hash ].string, string ) != 0 )
        {
            ++hash;
        }

        // Don’t set x or y position yet — this will come later.
        o.characters[ o.count ].src.w = o.characters[ o.count ].dest.w = characters[ hash ].w;
        o.characters[ o.count ].src.x = characters[ hash ].x;
        o.characters[ o.count ].src.y = characters[ hash ].y;
        o.characters[ o.count ].src.h = o.characters[ o.count ].dest.h = 8.0;
        o.characters[ o.count ].type = characters[ hash ].type;
        o.characters[ o.count ].color = args.color;
        ++o.count;
    }

    // 2nd loop: break characters into lines, broken by newlines or when a line reaches the width limit.
    character_t lines[ MAX_TEXT_LINES ][ MAX_CHARACTERS_PER_LINE ];
    double line_widths[ MAX_TEXT_LINES ];
    line_widths[ 0 ] = 0;
    int line_character_counts[ MAX_TEXT_LINES ];
    int line_count = 0;
    int line_character = 0;
    int i = 0;
    while ( i < o.count )
    {
        int ib = i;
        double xb = x;
        int look_ahead = 1;

        // Look ahead so we can know ahead o’ time whether we need to add a new line.
        // This autobreaks text without cutting midword.
        while ( look_ahead )
        {
            if ( ib >= o.count )
            {
                look_ahead = 0;
                break;
            }

            if ( o.characters[ ib ].type == CHARTYPE_NEWLINE )
            {
                look_ahead = 0;
            }
            else if ( o.characters[ ib ].type == CHARTYPE_WHITESPACE )
            {
                look_ahead = 0;
            }
            else if ( xb >= line_end )
            {
                x = args.x;
                line_character_counts[ line_count ] = line_character;
                ++line_count;
                line_widths[ line_count ] = 0;
                line_character = 0;
                look_ahead = 0;
            }
            else if ( ib >= o.count )
            {
                look_ahead = 0;
                break;
            }
            xb += o.characters[ ib ].src.w;
            ++ib;
        }

        while ( i < ib )
        {
            if ( o.characters[ i ].type == CHARTYPE_NEWLINE || x >= line_end )
            {
                x = args.x;
                line_character_counts[ line_count ] = line_character;
                ++line_count;
                line_widths[ line_count ] = 0;
                line_character = 0;
            }
            else
            {
                lines[ line_count ][ line_character ] = o.characters[ i ];
                line_widths[ line_count ] += o.characters[ i ].src.w;
            }
            ++i;
            ++line_character;
            x += o.characters[ i ].src.w;
        }
    }
    line_character_counts[ line_count ] = line_character;
    ++line_count;


    // Sometimes the previous loop keeps whitespace @ the end o’ lines.
    // Since this messes with x alignment, remove these.
    for ( int l = 0; l < line_count; ++l )
    {
        if ( lines[ l ][ line_character_counts[ l ] - 1 ].type == CHARTYPE_WHITESPACE )
        {
            --line_character_counts[ l ];
            line_widths[ l ] -= lines[ l ][ line_character_counts[ l ] - 1 ].src.w;
        }
    }

    // Final loop: we have all the info we need now to set x & y positions.
    i = 0;
    double y = ( args.valign == VALIGN_MIDDLE )
        ? args.y + ( ( args.h - ( line_count * 8.0 ) ) / 2.0 )
        : ( args.valign == VALIGN_BOTTOM )
            ? args.y + args.h - ( line_count * 8.0 )
            : args.y;
    for ( int l = 0; l < line_count; ++l )
    {
        x = ( args.align == ALIGN_CENTER )
            ? args.x + ( ( args.w - line_widths[ l ] ) / 2.0 )
            : ( args.align == ALIGN_RIGHT )
                ? line_end - line_widths[ l ]
                : args.x;
        for ( int c = 0; c < line_character_counts[ l ]; ++c )
        {
            // Just in case o’ character index misalignment, just copy o’er whole characters.
            o.characters[ i ] = lines[ l ][ c ];
            o.characters[ i ].dest.x = x;
            o.characters[ i ].dest.y = y;
            x += o.characters[ i ].dest.w;
            ++i;
        }
        y += 8.0;
    }

    // ’Cause we removed some characters after 1st loop, update count to fit new list.
    // Characters past count will just be ignored.
    o.count = i;

    return o;
};

int text_init()
{
    for ( int i = 0; i < MAX_CHARACTER_TYPES; ++i )
    {
        characters[ i ].filled = 0;
        characters[ i ].w = 8.0;
    }

    char * path = asset_localization( "en.json" );
    char * text = asset_read_file_text( path );
    free( path );
    if ( !text )
    {
        printf( "Couldn’t read English localization file.\n" );
        return -1;
    }
    json_char * json = ( json_char * )( text );
    char error[ 8000 ];
    json_settings settings = { 0 };
    json_value * root = json_parse_ex( &settings, json, strlen( text ), error );
    free( text );
    if ( !root )
    {
        printf( "Error parsing whole English localization json file.\n" );
        printf( "%s\n", error );
        return -1;
    }
    if ( root->type != json_object )
    {
        json_value_free( root );
        printf( "Error parsing English localization json file.\n" );
        return -1;
    }

    char * character_names[ 255 ];
    character_template_t character_types[ 255 ];
    int character_type_count = 0;

    for ( int i = 0; i < root->u.object.length; ++i )
    {
        const json_char * name = root->u.object.values[ i ].name;
        if ( strcmp( name, "characters" ) == 0 )
        {
            json_value * value = root->u.object.values[ i ].value;
            if ( value->type != json_array )
            {
                printf( "English localization json file is not formatted correctly.\n" );
                return -1;
            }
            for ( int j = 0; j < value->u.array.length; ++j )
            {
                character_types[ character_type_count ].w = 8.0;
                character_types[ character_type_count ].type = CHARTYPE_NORMAL;
                json_value * entry = value->u.array.values[ j ];
                if ( entry->type != json_object )
                {
                    printf( "English localization json file is not formatted correctly.\n" );
                    return -1;
                }
                for ( int k = 0; k < entry->u.object.length; ++k )
                {
                    const json_object_entry * col = &entry->u.object.values[ k ];
                    const json_char * col_name = col->name;
                    if ( strcmp( col_name, "key" ) == 0 )
                    {
                        if ( col->value->type != json_string )
                        {
                            printf( "English localization json file is not formatted correctly.\n" );
                            return -1;
                        }
                        character_names[ character_type_count ] = col->value->u.string.ptr;
                    }
                    else if ( strcmp( col_name, "values" ) == 0 )
                    {
                        if ( col->value->type == json_object )
                        {
                            for ( int l = 0; l < col->value->u.object.length; ++l )
                            {
                                const json_object_entry character_col = col->value->u.object.values[ l ];
                                if ( strcmp( character_col.name, "x" ) == 0 )
                                {
                                    if ( character_col.value->type == json_integer )
                                    {
                                        character_types[ character_type_count ].x = ( double )( character_col.value->u.integer );
                                    }
                                }
                                else if ( strcmp( character_col.name, "y" ) == 0 )
                                {
                                    if ( character_col.value->type == json_integer )
                                    {
                                        character_types[ character_type_count ].y = ( double )( character_col.value->u.integer );
                                    }
                                }
                                else if ( strcmp( character_col.name, "w" ) == 0 )
                                {
                                    if ( character_col.value->type == json_integer )
                                    {
                                        character_types[ character_type_count ].w = ( double )( character_col.value->u.integer );
                                    }
                                }
                                else if ( strcmp( character_col.name, "type" ) == 0 )
                                {
                                    if ( character_col.value->type == json_string )
                                    {
                                        if ( strcmp( character_col.value->u.string.ptr, "whitespace" ) == 0 )
                                        {
                                            character_types[ character_type_count ].type = CHARTYPE_WHITESPACE;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                ++character_type_count;
            }
        }
    }

    for ( int i = 0; i < character_type_count; ++i )
    {
        const char * text = character_names[ i ];
        int character_size = get_character_size( text );
        int hash = get_hash( text, character_size );
        while ( characters[ hash ].filled )
        {
            ++hash;
        }
        characters[ hash ] = character_types[ i ];
        characters[ hash ].filled = 1;
        
        strncpy( characters[ hash ].string, character_names[ i ], 5 );
    }

    json_value_free( root );
    return 0;
};

text_args_t text_default_args()
{
    text_args_t args =
    {
        0.0,
        0.0,
        ALIGN_LEFT,
        VALIGN_TOP,
        0.0,
        0.0,
        WINDOW_WIDTH_PIXELS,
        WINDOW_HEIGHT_PIXELS,
        { 0, 0, 0, 255 }
    };
    return args;
};

static get_character_size( const char * s )
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

static char * get_string_from_character( const character_t * c )
{
    for ( int i = 0; i < MAX_CHARACTER_TYPES; ++i )
    {
        if ( c->src.x == characters[ i ].x && c->src.y == characters[ i ].y )
        {
            return characters[ i ].string;
        }
    }
    return NULL;
};

static int get_hash( const char * text, int character_size )
{
    int hash = 5381;
    for ( int i = 0; i < character_size; ++i )
    {
        hash = ( 33 * hash ^ ( unsigned char )( *text++ ) ) % MAX_CHARACTER_TYPES;
    }
    return hash;
};
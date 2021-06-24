#include "character_template.h"
#include "filename.h"
#include "io.h"
#include "json/json.h"
#include "log.h"
#include "localization.h"
#include <string.h>

static Localization * localization_create( const char * code );

static AssocArray langs;
static Localization * current;

void localization_init()
{
    langs = assoc_array_create( -1 );
    current = localization_create( "en" );
    assoc_array_add( &langs, "en", value_create_unique_ptr( current ) );
};

void localization_close()
{
    for ( int i = 0; i < langs.capacity; ++i )
    {
        if ( langs.entries[ i ].key.string != NULL && langs.entries[ i ].value.value.ptr_ != NULL )
        {
            assoc_array_destroy( &(( Localization * )( langs.entries[ i ].value.value.ptr_ ))->charmap );
        }
    }
    assoc_array_destroy( &langs );
};

Localization * localization_get_current()
{
    return current;
};

Localization * localization_create( const char * code )
{
    Localization * lang = ( Localization * )( calloc( 1, sizeof( Localization * ) ) );
    lang->charmap = assoc_array_create( 255 );

    char * source = filename_localization( code );
    char * text = io_read( source );
    free( source );
    if ( !text )
    {
        log_error( "Couldn’t read English localization file.\n" );
        exit( -76 );
    }
    json_char * json = ( json_char * )( text );
    char error[ 8000 ];
    json_settings settings = { 0 };
    json_value * root = json_parse_ex( &settings, json, strlen( text ), error );
    free( text );
    if ( !root )
    {
        log_error( "Error parsing whole English localization json file." );
        log_error( error );
    }
    if ( root->type != json_object )
    {
        json_value_free( root );
        log_error( "Error parsing English localization json file.\n" );
    }

    for ( unsigned int i = 0; i < root->u.object.length; ++i )
    {
        const json_char * rootkey = root->u.object.values[ i ].name;
        if ( strcmp( rootkey, "characters" ) == 0 )
        {
            json_value * charsobj = root->u.object.values[ i ].value;
            if ( charsobj->type != json_array )
            {
                log_error( "English localization json file is not formatted correctly.\n" );
            }
            for ( unsigned int j = 0; j < charsobj->u.array.length; ++j )
            {
                json_value * charentry = charsobj->u.array.values[ j ];
                if ( charentry->type != json_object )
                {
                    log_error( "English localization json file is not formatted correctly.\n" );
                }

                char * key = NULL;
                json_value * value = NULL;

                for ( unsigned int k = 0; k < charentry->u.object.length; ++k )
                {
                    json_object_entry * parameter = &charentry->u.object.values[ k ];
                    if ( strcmp( parameter->name, "key" ) == 0 && parameter->value->type == json_string )
                    {
                        key = parameter->value->u.string.ptr;
                    }
                    else if ( strcmp( parameter->name, "value" ) == 0 && parameter->value->type == json_object )
                    {
                        value = parameter->value;
                    }
                }

                if ( key == NULL || value == NULL )
                {
                    log_error( "English localization json file is not formatted correctly.\n" );
                }

                CharacterTemplate c;
                c.w = 8.0;
                c.h = 8.0;
                c.type = CHAR_NORMAL;

                for ( unsigned int k = 0; k < value->u.object.length; ++k )
                {
                    json_object_entry * parameter = &value->u.object.values[ k ];
                    if ( strcmp( parameter->name, "x" ) == 0 )
                    {
                        if ( parameter->value->type != json_integer )
                        {
                            log_error( "English localization json file is not formatted correctly.\n" );
                        }
                        c.x = ( double )( parameter->value->u.integer );
                    }
                    else if ( strcmp( parameter->name, "y" ) == 0 )
                    {
                        if ( parameter->value->type != json_integer )
                        {
                            log_error( "English localization json file is not formatted correctly.\n" );
                        }
                        c.y = ( double )( parameter->value->u.integer );
                    }
                    else if ( strcmp( parameter->name, "w" ) == 0 )
                    {
                        if ( parameter->value->type != json_integer )
                        {
                            log_error( "English localization json file is not formatted correctly.\n" );
                        }
                        c.w = ( double )( parameter->value->u.integer );
                    }
                    else if ( strcmp( parameter->name, "h" ) == 0 )
                    {
                        if ( parameter->value->type != json_integer )
                        {
                            log_error( "English localization json file is not formatted correctly.\n" );
                        }
                        c.h = ( double )( parameter->value->u.integer );
                    }
                    else if ( strcmp( parameter->name, "type" ) == 0 )
                    {
                        if ( parameter->value->type != json_string )
                        {
                            log_error( "English localization json file is not formatted correctly.\n" );
                        }
                        if ( strcmp( parameter->value->u.string.ptr, "whitespace" ) == 0 )
                        {
                            c.type = CHAR_WHITESPACE;
                        }
                        else if ( strcmp( parameter->value->u.string.ptr, "newline" ) == 0 )
                        {
                            c.type = CHAR_NEWLINE;
                        }
                        else if ( strcmp( parameter->value->u.string.ptr, "normal" ) != 0 )
                        {
                            log_error( "English localization json file is not formatted correctly.\n" );
                        }
                    }
                }

                void * cp = malloc( sizeof( CharacterTemplate ) );
                memcpy( cp, &c, sizeof( CharacterTemplate ) );
                assoc_array_add( &lang->charmap, key, value_create_unique_ptr( cp ) );
            }
        }
    }

    json_value_free( root );
    return lang;
};
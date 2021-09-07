#include "io.hpp"
#include "localization.hpp"
#include "log.hpp"
#include "json/json.h"
#include <vector>

namespace Localization
{
    LocalizationLanguage lang;

    const LocalizationLanguage & getCurrentLanguage()
    {
        return lang;
    };

    bool init()
    {
        const std::string slug = "en";
        const std::string contents = IO::read( "assets/localization/" + slug + ".json" );
        json_char * json = ( json_char * )( contents.c_str() );
        json_value * root = json_parse( json, strlen( contents.c_str() ) + 1 );
        if ( root == nullptr || root->type != json_object )
        {
            Log::sendError( "Failed to parse JSON for localization file “" + slug + "”." );
            return false;
        }

        for ( unsigned int i = 0; i < root->u.object.length; ++i )
        {
            const json_object_entry root_entry = root->u.object.values[ i ];
            if ( strcmp( "characters", root_entry.name ) == 0 )
            {
                if ( root_entry.value->type != json_array )
                {
                    Log::sendError( "Localization file “" + slug + "” malformed: characters is not an array." );
                    return false;
                }

                for ( unsigned int j = 0; j < root_entry.value->u.array.length; ++j )
                {
                    const json_value * char_item = root_entry.value->u.array.values[ j ];
                    if ( char_item->type != json_object )
                    {
                        Log::sendError( "Localization file “" + slug + "” malformed: character entry not an object." );
                        return false;
                    }

                    CharacterTemplate c { CharacterType::NORMAL, 8.0f, 8.0f, 0.0f, 0.0f };
                    std::string key = "";

                    std::unordered_map<std::string, float *> float_props
                    {
                        { "x", &c.x },
                        { "y", &c.y },
                        { "w", &c.w }
                    };

                    for ( unsigned int k = 0; k < char_item->u.object.length; ++k )
                    {
                        const json_object_entry char_entry = char_item->u.object.values[ k ];
                        if ( strcmp( "key", char_entry.name ) == 0 )
                        {
                            if ( char_entry.value->type != json_string )
                            {
                                Log::sendError( "Localization file “" + slug + "” malformed: character key is not a string." );
                                return false;
                            }
                            key = std::string( char_entry.value->u.string.ptr );
                        }
                        else if ( strcmp( "type", char_entry.name ) == 0 )
                        {
                            if ( char_entry.value->type != json_string )
                            {
                                Log::sendError( "Localization file “" + slug + "” malformed: character type is not a string." );
                                return false;
                            }
                            if ( strcmp( "whitespace", char_entry.value->u.string.ptr ) == 0 )
                            {
                                c.type = CharacterType::WHITESPACE;
                            }
                        }
                        // Iterate o’er all float props, test if in it, & set according prop in c.
                        else
                        {
                            auto it = float_props.begin();
                            while ( it != float_props.end() )
                            {
                                if ( strcmp( it->first.c_str(), char_entry.name ) == 0 )
                                {
                                    if ( char_entry.value->type == json_integer )
                                    {
                                        *it->second = ( float )( char_entry.value->u.integer );
                                    }
                                    else if ( char_entry.value->type == json_double )
                                    {
                                        *it->second = ( float )( char_entry.value->u.dbl );
                                    }
                                    else
                                    {
                                        Log::sendError( "Localization file “" + slug + "” malformed: character " + it->first + " is not an int." );
                                        return false;
                                    }
                                }
                                ++it;
                            }
                        }
                    }
                    lang.chars.insert( std::pair<std::string, CharacterTemplate>( key, c ) );
                }

            }
        }

        json_value_free( root );
        return true;
    };
};
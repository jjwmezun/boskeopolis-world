#include <fstream>
#include "filename.hpp"
#include "localization_language.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

LocalizationLanguage::LocalizationLanguage( const char * code )
{
    const std::string full_filename = std::string( filename_localization( code ) );
    std::ifstream ifs( full_filename );
    if( !ifs.is_open() )
    {
        throw std::runtime_error( "Unexpected IO error for loading localization: somehow file we found in localization directory failed to be opened." );
    }
    rapidjson::IStreamWrapper ifs_wrap( ifs );
    rapidjson::Document document;
    document.ParseStream( ifs_wrap );
    ifs.close();

    if ( !document.IsObject() )
    {
        throw std::runtime_error( "Invalid localization file" );
    }

    const auto & obj = document.GetObject();
    if ( !obj.HasMember( "characters" ) || !obj[ "characters" ].IsArray() )
    {
        throw std::runtime_error( "Invalid localization file" );
    }

    for ( auto & character : obj[ "characters" ].GetArray() )
    {
        if ( !character.IsObject() )
        {
            throw std::runtime_error( "Invalid localization file" );
        }

        const auto & cobj = character.GetObject();
        if
        (
            !cobj.HasMember( "key" ) || !cobj[ "key" ].IsString() ||
            !cobj.HasMember( "value" ) || !cobj[ "value" ].IsObject()
        )
        {
            throw std::runtime_error( "Invalid localization file" );
        }

        const auto & vobj = cobj[ "value" ].GetObject();
        CharacterTemplate c;
        c.w = 8.0;
        c.h = 8.0;
        c.type = CharacterTemplate::Type::NORMAL;

        if ( vobj.HasMember( "x" ) && vobj[ "x" ].IsInt() )
        {
            c.x = ( double )( vobj[ "x" ].GetInt() );
        }
        if ( vobj.HasMember( "y" ) && vobj[ "y" ].IsInt() )
        {
            c.y = ( double )( vobj[ "y" ].GetInt() );
        }
        if ( vobj.HasMember( "w" ) && vobj[ "w" ].IsInt() )
        {
            c.w = ( double )( vobj[ "w" ].GetInt() );
        }
        if ( vobj.HasMember( "h" ) && vobj[ "h" ].IsInt() )
        {
            c.h = ( double )( vobj[ "h" ].GetInt() );
        }
        if ( vobj.HasMember( "type" ) && vobj[ "type" ].IsString() )
        {
            std::string type = vobj[ "type" ].GetString();
            if ( type == "whitespace" )
            {
                c.type = CharacterTemplate::Type::WHITESPACE;
            }
            else if ( type == "newline" )
            {
                c.type = CharacterTemplate::Type::NEWLINE;
            }
        }

        character_map_.insert( std::pair<std::string, CharacterTemplate> { std::string( cobj[ "key" ].GetString() ), c } );
    }
};

const std::unordered_map<std::string, CharacterTemplate>& LocalizationLanguage::getCharacterMap() const
{
    return character_map_;
};
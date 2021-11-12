#include "io.hpp"
#include "jjson.hpp"
#include "log.hpp"

namespace JJSON
{
    json_value * load( std::string file, std::string error_message )
    {
        const std::string contents = IO::read( file );
        json_char * json = ( json_char * )( contents.c_str() );
        json_value * root = json_parse( json, strlen( contents.c_str() ) + 1 );
        if ( root == nullptr || root->type != json_object )
        {
            Log::sendError( error_message );
        }
        return root;
    };
};
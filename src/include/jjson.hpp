#ifndef JJSON_H
#define JJSON_H

#include "json/json.h"
#include <string>

namespace JJSON
{
    json_value * load( std::string file, std::string error_message );
};

#endif // JJSON_H
#include "dir.hpp"
#include <regex>
#include "utility.hpp"

namespace BSW
{
    namespace Dir
    {
        XY getXYFromString( std::string key )
        {
            // Remove _ or - to accommodate them being used as space ’tween direction words.
            std::regex reg = std::regex( "_|-" );
            key = std::regex_replace( key, reg, "" );

            return BSW::findInMap<XY>
            (
                {
                    { "up", XY::UP },
                    { "upright", XY::UP_RIGHT },
                    { "right", XY::RIGHT },
                    { "downright", XY::DOWN_RIGHT },
                    { "down", XY::DOWN },
                    { "downleft", XY::DOWN_LEFT },
                    { "left", XY::LEFT },
                    { "upleft", XY::UP_LEFT }
                },
                key,
                "getXYFromString Error: Invalid direction “" + key + "”."
            );
        };
    }
}
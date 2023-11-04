#ifndef DIR_H
#define DIR_H

#include <string>

namespace BSW
{
    namespace Dir
    {
        enum class XY
        {
            UP         = 0,
            UP_RIGHT   = 1,
            RIGHT      = 2,
            DOWN_RIGHT = 3,
            DOWN       = 4,
            DOWN_LEFT  = 5,
            LEFT       = 6,
            UP_LEFT    = 7,
            NONE       = 8
        };

        enum class X
        {
            RIGHT = static_cast<int>( XY::RIGHT ),
            LEFT = static_cast<int>( XY::LEFT ),
            NONE = static_cast<int>( XY::NONE )
        };

        enum class Y
        {
            UP = static_cast<int>( XY::UP ),
            DOWN = static_cast<int>( XY::DOWN ),
            NONE = static_cast<int>( XY::NONE )
        };

        XY getXYFromString( std::string key );
    }
}

#endif // DIR_H
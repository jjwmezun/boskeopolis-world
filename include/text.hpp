#ifndef TEXT_H
#define TEXT_H

namespace BSW
{
    namespace Text
    {
        enum class Align
        {
            DEFAULT   = 0,
            LEFT      = 1,
            RIGHT     = 2,
            CENTER    = 3,
            JUSTIFIED = 4
        };

        enum class Valign
        {
            DEFAULT = 0,
            TOP     = 1,
            MIDDLE  = 2,
            BOTTOM  = 3
        };
    }
}

#endif // TEXT_H
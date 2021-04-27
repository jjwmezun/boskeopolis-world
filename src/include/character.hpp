#ifndef CHARACTER_H
#define CHARACTER_H

#include "color.hpp"
#include "rect.hpp"

struct Character
{
    enum class Type
    {
        NORMAL,
        WHITESPACE,
        NEWLINE
    };

    Type type;
    Color color;
    Rect src;
    Rect dest;
};

#endif // CHARACTER_H
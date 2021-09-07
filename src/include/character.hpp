#ifndef CHARACTER_H
#define CHARACTER_H

#include "color.hpp"

struct Character
{
    float w;
    float h;
    float src_x;
    float src_y;
    float dest_x;
    float dest_y;
    Color color;
};

#endif // CHARACTER_H
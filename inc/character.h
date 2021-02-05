#ifndef CHARACTER_H
#define CHARACTER_H

#include "color.h"
#include "rect.h"

typedef enum character_type_t
{
    CHARTYPE_NORMAL,
    CHARTYPE_WHITESPACE,
    CHARTYPE_NEWLINE
} character_type_t;

typedef struct character_t
{
    rect_t src;
    rect_t dest;
    color_t color;
    character_type_t type;
} character_t;

#endif
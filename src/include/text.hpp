#ifndef TEXT_H
#define TEXT_H

#include "assoc_array.hpp"
#include "character.hpp"
#include "color.hpp"

enum TextAlign
{
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT
};

enum TextVAlign
{
    VALIGN_TOP,
    VALIGN_MIDDLE,
    VALIGN_BOTTOM
};

#define MAX_CHARACTERS 256

typedef struct
{
    Character characters[ MAX_CHARACTERS ];
    int number_of_characters;
    Color color;
}
Text;

Text text_create( const char * text, AssocArray * args );

#endif // TEXT_H
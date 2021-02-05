#ifndef TEXT_H
#define TEXT_H

#include "character.h"
#include "color.h"

#define MAX_TEXT_CHARACTERS 256

typedef enum text_align_t
{
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT
} text_align_t;

typedef enum text_valign_t
{
    VALIGN_TOP,
    VALIGN_MIDDLE,
    VALIGN_BOTTOM
} text_valign_t;

typedef struct text_args_t
{
    double x;
    double y;
    text_align_t align;
    text_valign_t valign;
    double x_padding;
    double y_padding;
    double w;
    double h;
    color_t color;
} text_args_t;

typedef struct text_t
{
    int count;
    character_t characters[ MAX_TEXT_CHARACTERS ];
} text_t;

text_t text_create( const char * text, text_args_t args );
int text_init();
text_args_t text_default_args();

#endif TEXT_H
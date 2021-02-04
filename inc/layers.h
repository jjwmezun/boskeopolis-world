#ifndef LAYERS_H
#define LAYERS_H

typedef enum layer_t
{
    LAYER_BG1,
    LAYER_BG2,
    LAYER_OBJ1,
    LAYER_SPR1,
    LAYER_BG3,
    LAYER_FG1,
    LAYER_OBJ2,
    LAYER_SPR2,
    LAYER_FG2,
    LAYER_FG3
} layer_t;

#define NUMBER_OF_LAYERS ( LAYER_FG3 + 1 )

#endif
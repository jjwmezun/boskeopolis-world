#ifndef UNIT_H
#define UNIT_H

#include <math.h>

#define PIXELS_PER_BLOCK 16
#define BLOCKS_TO_PIXELS( n ) ( ( n ) * PIXELS_PER_BLOCK )
#define PIXELS_TO_BLOCKS( n ) ( floor( ( n ) / PIXELS_PER_BLOCK ) )

#define WINDOW_WIDTH_BLOCKS 25
#define WINDOW_HEIGHT_BLOCKS 14
#define WINDOW_WIDTH_PIXELS BLOCKS_TO_PIXELS( WINDOW_WIDTH_BLOCKS )
#define WINDOW_HEIGHT_PIXELS BLOCKS_TO_PIXELS( WINDOW_HEIGHT_BLOCKS )

#define DEGREES_TO_RADIANS( n ) n * 3.14159 / 180.0

typedef enum
{
	LAYER_BEFORE_BG_1 = 0,
	LAYER_BG_1,
	LAYER_BEFORE_BG_2,
	LAYER_BG_2,
	LAYER_BEFORE_BLOCKS_1,
	LAYER_BLOCKS_1,
	LAYER_BEFORE_SPRITES_1,
	LAYER_SPRITES_1,
	LAYER_BEFORE_BLOCKS_2,
	LAYER_BLOCKS_2,
	LAYER_BEFORE_SPRITES_2,
	LAYER_SPRITES_2,
	LAYER_BEFORE_FG_1,
	LAYER_FG_1,
	LAYER_BEFORE_FG_2,
	LAYER_FG_2,
	LAYER_AFTER_FG_2,
	LAYER_SUPER
}
Layer;

#define NUMBER_OF_LAYERS LAYER_SUPER + 1

#endif // UNIT_H
#ifndef CONFIG_H
#define CONFIG_H

#include <math.h>

#define BLOCK_SIZE 16
#define BLOCKS_TO_PIXELS( blocks ) ( ( blocks ) * BLOCK_SIZE )
#define PIXELS_TO_BLOCKS( pixels ) ( ( pixels ) / BLOCK_SIZE )
#define WINDOW_WIDTH_BLOCKS 25
#define WINDOW_HEIGHT_BLOCKS 14
#define WINDOW_WIDTH_PIXELS ( WINDOW_WIDTH_BLOCKS * BLOCK_SIZE )
#define WINDOW_HEIGHT_PIXELS ( WINDOW_HEIGHT_BLOCKS * BLOCK_SIZE )
#define FPS_MILLISECONDS ( int )( floor( 1000.0 / 60.0 ) )

#endif
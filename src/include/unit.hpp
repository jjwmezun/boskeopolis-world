#ifndef UNIT_H
#define UNIT_H

#include <cmath>

namespace Unit
{
    static constexpr int PIXELS_PER_BLOCK = 16;
    static constexpr int WINDOW_WIDTH_BLOCKS = 25;
    static constexpr int WINDOW_HEIGHT_BLOCKS = 14;
    static constexpr int blocksToPixels( int n ) { return n * PIXELS_PER_BLOCK; };
    static constexpr int pixelsToBlocks( int n ) { return ( int )( floor( ( double )( n ) / ( double )( PIXELS_PER_BLOCK ) ) ); };
    static constexpr int WINDOW_WIDTH_PIXELS = blocksToPixels( WINDOW_WIDTH_BLOCKS );
    static constexpr int WINDOW_HEIGHT_PIXELS = blocksToPixels( WINDOW_HEIGHT_BLOCKS );
    static constexpr double degreesToRadians( float n ) { return n * 3.14159 / 180.0; };
    static constexpr int MAX_STATES = 5;
}

#endif // UNIT_H
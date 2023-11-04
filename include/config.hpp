#ifndef CONFIG_H
#define CONFIG_H

namespace BSW
{
    constexpr int MAX_STATES = 5;
    constexpr int BLOCK_SIZE = 16;
    constexpr int WINDOW_WIDTH_BLOCKS = 30;
    constexpr int WINDOW_HEIGHT_BLOCKS = 17;
    constexpr int WINDOW_WIDTH_PIXELS = WINDOW_WIDTH_BLOCKS * BLOCK_SIZE;
    constexpr int WINDOW_HEIGHT_PIXELS = WINDOW_HEIGHT_BLOCKS * BLOCK_SIZE;

    constexpr float pixelsToBlocks( float p )
    {
        return p / ( float )( BLOCK_SIZE );
    };

    constexpr int pixelsToBlocks( int p )
    {
        return p / BLOCK_SIZE;
    };

    constexpr int blocksToPixels( int b )
    {
        return b * BLOCK_SIZE;
    };
}

#endif // CONFIG_H
#ifndef UNIT_H
#define UNIT_H

#include <algorithm>
#include <cmath>

namespace Unit
{
    static constexpr int PIXELS_PER_BLOCK = 16;

    static constexpr int blocksToPixels( int n )
    {
        return n * PIXELS_PER_BLOCK;
    };

    static constexpr int pixelsToBlocks( int n )
    {
        return floor( n / PIXELS_PER_BLOCK );
    };

    static constexpr int WINDOW_WIDTH_BLOCKS = 25;
    static constexpr int WINDOW_HEIGHT_BLOCKS = 14;
    static constexpr int WINDOW_WIDTH_PIXELS = blocksToPixels( WINDOW_WIDTH_BLOCKS );
    static constexpr int WINDOW_HEIGHT_PIXELS = blocksToPixels( WINDOW_HEIGHT_BLOCKS );

    enum class Layer
	{
		BEFORE_BG_1 = 0,
		BG_1,
		BEFORE_BG_2,
		BG_2,
		BEFORE_BLOCKS_1,
		BLOCKS_1,
		BEFORE_SPRITES_1,
		SPRITES_1,
		BEFORE_BLOCKS_2,
		BLOCKS_2,
		BEFORE_SPRITES_2,
		SPRITES_2,
		BEFORE_FG_1,
		FG_1,
		BEFORE_FG_2,
		FG_2,
		AFTER_FG_2,
		SUPER
	};
	static constexpr int NUMBER_OF_LAYERS = ( int )( Layer::SUPER ) + 1;

	inline constexpr Layer intToLayer( int value )
	{
		return ( Layer )( std::max( 0, std::min( NUMBER_OF_LAYERS, value ) ) );
	};
}

#endif // UNIT_H
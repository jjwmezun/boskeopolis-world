#ifndef LAYER_H
#define LAYER_H

#include <optional>
#include <string>

namespace BSW
{
    enum class Layer
    {
        BEFORE_BG_1 = 0,
        BG_1 = 1,
        BEFORE_BG_2 = 2,
        BG_2 = 3,
        BEFORE_BLOCKS_1 = 4,
        BLOCKS_1 = 5,
        BEFORE_SPRITES_1 = 6,
        SPRITES_1 = 7,
        BEFORE_BLOCKS_2 = 8,
        BLOCKS_2 = 9,
        BEFORE_SPRITES_2 = 10,
        SPRITES_2 = 11,
        BEFORE_FG_1 = 12,
        FG_1 = 13,
        BEFORE_FG_2 = 14,
        FG_2 = 15,
        AFTER_FG_2 = 16,
        SUPER = 17
    };

    std::optional<Layer> getLayerFromString( const std::string & string );
}

#endif // LAYER_H
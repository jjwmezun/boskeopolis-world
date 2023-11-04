#include "layer.hpp"
#include <unordered_map>

namespace BSW
{
    static constexpr int MAX_LAYERS = 18;
    static const std::unordered_map<std::string, Layer> layermap
    {
        { "BEFORE_BG_1", Layer::BEFORE_BG_1 },
        { "BG_1", Layer::BG_1 },
        { "BEFORE_BG_2", Layer::BEFORE_BG_2 },
        { "BG_2", Layer::BG_2 },
        { "BEFORE_BLOCKS_1", Layer::BEFORE_BLOCKS_1 },
        { "BLOCKS_1", Layer::BLOCKS_1 },
        { "BEFORE_SPRITES_1", Layer::BEFORE_SPRITES_1 },
        { "SPRITES_1", Layer::SPRITES_1 },
        { "BEFORE_BLOCKS_2", Layer::BEFORE_BLOCKS_2 },
        { "BLOCKS_2", Layer::BLOCKS_2 },
        { "BEFORE_SPRITES_2", Layer::BEFORE_SPRITES_2 },
        { "SPRITES_2", Layer::SPRITES_2 },
        { "BEFORE_FG_1", Layer::BEFORE_FG_1 },
        { "FG_1", Layer::FG_1 },
        { "BEFORE_FG_2", Layer::BEFORE_FG_2 },
        { "FG_2", Layer::FG_2 },
        { "AFTER_FG_2", Layer::AFTER_FG_2 },
        { "SUPER", Layer::SUPER }
    };

    std::optional<Layer> getLayerFromString( const std::string & string )
    {
        const auto it = layermap.find( string );
        if ( it == layermap.end() )
        {
            return {};
        }
        return it->second;
    };
}
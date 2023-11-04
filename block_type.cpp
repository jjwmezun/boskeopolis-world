#include "block_type.hpp"

namespace BSW
{
    BlockType::BlockType()
        : is_null_ ( true ), x_ ( 0 ), y_ ( 0 ), animation_ ( 0 ), palette_ ( 0 ) {};

    BlockType::BlockType
    (
        unsigned char x,
        unsigned char y,
        unsigned char animation,
        unsigned char palette,
        const std::vector<BlockBehavior> & behaviors
    )
    :
        is_null_ ( false ),
        x_ ( x ),
        y_ ( y ),
        animation_ ( animation ),
        palette_ ( palette ),
        behaviors_ ( behaviors )
    {};

    bool BlockType::isNull() const
    {
        return is_null_;
    };

    unsigned char BlockType::getX() const { return x_; };
    unsigned char BlockType::getY() const { return y_; };
    unsigned char BlockType::getAnimation() const { return animation_; };
    unsigned char BlockType::getPalette() const { return palette_; };

    void BlockType::interact( Sprite & sprite, Level & level, Game & game, Map & map, Block & block ) const
    {
        for ( const BlockBehavior & behavior : behaviors_ )
        {
            behavior.interact( sprite, level, game, map, block );
        }
    };
}
#include "block_behavior.hpp"

namespace BSW
{
    void BlockBehavior::interact( Sprite & sprite, Level & level, Game & game, Map & map, Block & block ) const
    {
        for ( const BlockCondition & condition : conditions_ )
        {
            if ( !condition.test( sprite, level, game, map, block ) ) return;
        }
        component_.interact( sprite, level, game, map, block );
    };
}
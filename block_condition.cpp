#include "block_condition.hpp"

namespace BSW
{
    BlockCondition::BlockCondition( Type type, const ArgList & args )
        : type_ ( type )
    {};

    bool BlockCondition::test
    (
        const Sprite & sprite,
        const Level & level,
        const Game & game,
        const Map & map,
        const Block & block
    ) const
    {
        switch ( type_ )
        {
            case ( Type::PROTAG ):
            {
                //return sprite.hasAttribute( Sprite::Attribute::PROTAG );
            }
            break;
        }

        return true;
    };
}
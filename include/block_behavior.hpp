#ifndef BLOCK_BEHAVIOR_H
#define BLOCK_BEHAVIOR_H

#include "block_component.hpp"
#include "block_condition.hpp"
#include <vector>

namespace BSW
{
    class Block;
    class Game;
    class Level;
    class Map;
    class Sprite;

    class BlockBehavior
    {
        public:
            inline BlockBehavior( const BlockComponent & component, const std::vector<BlockCondition> & conditions )
            :
                component_ ( component ),
                conditions_ ( conditions )
            {};

            void interact( Sprite & sprite, Level & level, Game & game, Map & map, Block & block ) const;

        private:
            const BlockComponent component_;
            std::vector<BlockCondition> conditions_;
    };
}

#endif // BLOCK_BEHAVIOR_H
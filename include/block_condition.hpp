#ifndef BLOCK_CONDITION_H
#define BLOCK_CONDITION_H

#include "arg.hpp"

namespace BSW
{
    class Block;
    class Game;
    class Level;
    class Map;
    class Sprite;

    class BlockCondition
    {
        public:
            enum class Type
            {
                PROTAG
            };

            BlockCondition( Type type, const ArgList & args );
            bool test
            (
                const Sprite & sprite,
                const Level & level,
                const Game & game,
                const Map & map,
                const Block & block
            ) const;

        private:
            Type type_;
    };
}

#endif // BLOCK_CONDITION_H
#ifndef BLOCK_COMPONENT_H
#define BLOCK_COMPONENT_H

#include "arg.hpp"

namespace BSW
{
    class Block;
    class Game;
    class Level;
    class Map;
    class Sprite;

    class BlockComponent
    {
        public:
            enum class Type
            {
                MONEY,
                REMOVE
            };

            BlockComponent( Type type, const ArgList & args );
            void interact( Sprite & sprite, Level & level, Game & game, Map & map, Block & block ) const;

        private:
            Type type_;
            union
            {
                struct { float amount; } money;
            } data_;
    };
}

#endif // BLOCK_COMPONENT_H
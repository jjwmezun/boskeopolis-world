#ifndef TILESET_H
#define TILESET_H

#include "block_type.hpp"
#include <string>

namespace BSW
{
    class Tileset
    {
        public:
            inline Tileset( std::string name ) : name_ ( name ) {};
            void init();
            const BlockType & getBlockType( unsigned int type ) const;

        private:
            std::vector<BlockType> data_;
            std::string name_;
    };
}

#endif // TILESET_H
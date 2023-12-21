#ifndef TILESET_H
#define TILESET_H

#include "arg.hpp"
#include <optional>
#include <string>
#include "tile.hpp"
#include <unordered_map>

namespace BSW
{
    struct BehaviorInfo
    {
        std::string type;
        ArgList args;
    };

    class Tileset
    {
        public:
            inline Tileset( std::string name ) : name_ ( name ) {};
            void init();
            const std::optional<Tile> getTile( unsigned int type ) const;
            const std::optional<BehaviorInfo> getBehaviorInfo( unsigned int type ) const;

        private:
            std::unordered_map<unsigned int, Tile> tiles_;
            std::unordered_map<unsigned int, BehaviorInfo> behavior_info_;
            std::string name_;
    };
}

#endif // TILESET_H
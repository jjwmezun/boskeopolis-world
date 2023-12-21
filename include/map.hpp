#ifndef MAP_H
#define MAP_H

#include "config.hpp"
#include "rect.hpp"
#include "renderer.hpp"
#include "tileset.hpp"

namespace BSW
{
    class Game;
    class Inventory;
    class Sprite;

    static constexpr unsigned int MAXWARPS = 3;

    enum class WarpType
    {
        __NULL = 0,
        NORMAL,
        CLIMB
    };

    struct Warp
    {
        WarpType type;
        unsigned int map;
        Rect coords;
        float entrance_x;
        float entrance_y;
        float camera_x;
        float camera_y;
    };

    enum class CollisionType
    {
        NONE = 0,
        SOLID = 1,
        SOLID_ABOVE = 2,
        CLIMB = 3,
        WARP = 4
    };

    enum class BlockType
    {
        __NULL = 0,
        MONEY
    };

    struct Block
    {
        BlockType type;
        unsigned int layer;
        union
        {
            struct
            {
                float amount;
            } money;
        } data;
    };

    struct BlockList
    {
        unsigned int count;
        Block * list;
    };

    struct Map
    {
        bool remove_block_;
        unsigned int width_;
        unsigned int height_;
        unsigned int i_;
        unsigned int warpcount_;
        Warp * warps_;
        CollisionType ** collision_;
        BlockList * blocks_;
        TilemapGraphics * block_layers_;

        constexpr unsigned int getWidthBlocks() const { return width_; };
        constexpr unsigned int getHeightBlocks() const { return height_; };
        constexpr unsigned int getWidthPixels() const { return blocksToPixels( width_ ); };
        constexpr unsigned int getHeightPixels() const { return blocksToPixels( height_ ); };
        constexpr unsigned int getIFromXAndY( unsigned int x, unsigned int y ) const
        {
            return y * width_ + x;
        };
        void init( const Game & game );
        void close();
        void interact( Sprite & sprite, Game & game, Inventory & inventory );
        bool testCollision( unsigned int x, unsigned int y, CollisionType type = CollisionType::SOLID ) const;
    };
};

#endif // MAP_H
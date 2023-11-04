#ifndef MAP_H
#define MAP_H

#include "config.hpp"
#include "rect.hpp"

namespace BSW
{
    class Game;

    enum class WarpType
    {
        NORMAL,
        CLIMB
    };

    struct Warp
    {
        WarpType type = WarpType::NORMAL;
        unsigned int map = 0;
        Rect coords = { 0.0f, 0.0f, 0.0f, 0.0f };
        float entrance_x = 0.0f;
        float entrance_y = 0.0f;
        float camera_x = 0.0f;
        float camera_y = 0.0f;
    };

    enum class CollisionType
    {
        NONE = 0,
        SOLID = 1,
        SOLID_ABOVE = 2,
        CLIMB = 3,
        WARP = 4
    };

    struct CollisionNode
    {
        CollisionType type = CollisionType::NONE;
        CollisionNode * next = nullptr;
    };

    struct Map
    {
        unsigned int width_;
        unsigned int height_;
        unsigned int warpcount_;
        Warp * warps_;
        CollisionNode * collision_;

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
        bool testCollision( unsigned int x, unsigned int y, CollisionType type = CollisionType::SOLID ) const;
    };
};

#endif // MAP_H
#include "tile.hpp"

namespace BSW
{
    Tile::Tile()
        : set_ ( false ), x_ ( 0 ), y_ ( 0 ), animation_ ( 0 ), palette_ ( 0 ) {};

    Tile::Tile
    (
        unsigned char x,
        unsigned char y,
        unsigned char animation,
        unsigned char palette
    )
    :
        set_ ( true ),
        x_ ( x ),
        y_ ( y ),
        animation_ ( animation ),
        palette_ ( palette )
    {};

    bool Tile::isNull() const
    {
        return !set_;
    };

    unsigned char Tile::getX() const { return x_; };
    unsigned char Tile::getY() const { return y_; };
    unsigned char Tile::getAnimation() const { return animation_; };
    unsigned char Tile::getPalette() const { return palette_; };
}
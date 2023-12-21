#ifndef TILE_H
#define TILE_H

namespace BSW
{
    class Tile
    {
        public:
            Tile();
            Tile
            (
                unsigned char x,
                unsigned char y,
                unsigned char animation,
                unsigned char palette
            );
            bool isNull() const;
            unsigned char getX() const;
            unsigned char getY() const;
            unsigned char getAnimation() const;
            unsigned char getPalette() const;

        private:
            bool set_;
            unsigned char x_;
            unsigned char y_;
            unsigned char animation_;
            unsigned char palette_;
    };
}

#endif // TILE_H
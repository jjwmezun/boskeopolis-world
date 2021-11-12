#ifndef TILE_H
#define TILE_H

struct Tile
{
    bool empty;
    unsigned char x;
    unsigned char y;
    unsigned char palette;
    unsigned char animation;
};

#endif // TILE_H
#ifndef RENDER_H
#define RENDER_H

#include "color.hpp"
#include "graphic.hpp"
#include "layer.hpp"
#include "unit.hpp"

namespace Render
{
    int init();
    void close();
    void update();
    unsigned int getTextureID( const char * local );
    unsigned int getTextureIDNoIndex( const char * local );
    void clearTextures();
    int shouldWindowClose();
    void * getWindow();
    void setStatesNumber( int number );
    void clearGraphics();
    void clearStateGraphics( int state );
    unsigned int addGraphic( Graphic gfx, int state, Layer layer );
    void removeGraphic( unsigned int id );
    Graphic * getGraphic( unsigned int id );
    void adjustCamera( Rect * target, float max_w, float max_h );
    unsigned int addTilemap( const char * tileset, const int * tiles, int w, int h, unsigned int pal, int state_number, Layer layer );
}

#endif // RENDER_H
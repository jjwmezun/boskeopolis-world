#ifndef RENDER_H
#define RENDER_H

#include "unit.hpp"

class Character;
class Color;
class Graphic;
class Rect;
class Text;

namespace Render
{
    bool init( const char * title, int width, int height, Color background );
    void close();
    void update();
    unsigned int getTextureId( const char * local, bool indexed = true );
    void clearTextures();
    bool windowShouldClose();
    void * getWindow();
    void setNumberOfStates( int number );
    void clearGraphics();
    void clearStateGraphics( int state );
    void addGraphic( Graphic gfx, int state, Unit::Layer layer );
}

#endif // RENDER_H
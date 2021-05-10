#ifndef RENDER_H
#define RENDER_H

#include "unit.hpp"

class Character;
class Color;
class Graphics;
class Rect;

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

    void addGraphic( Graphics gfx, int state, Unit::Layer layer );
    void rect( const Rect & rect, const Color & color );
    void sprite( unsigned int texture_id, unsigned int palette_id, const Rect & src, const Rect & dest, bool flip_x, bool flip_y, float rotation_x, float rotation_y, float rotation_z );
    void character( const Character & character, const Color & color );
    void colorCanvas( const Color & color );
}

#endif // RENDER_H
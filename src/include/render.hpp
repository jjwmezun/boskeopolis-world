#ifndef RENDER_H
#define RENDER_H

class Character;
class Color;
class Rect;

namespace Render
{
    bool init( const char * title, int width, int height, Color background );
    void close();
    void startUpdate();
    void endUpdate();
    unsigned int getTextureId( const char * local, bool indexed = true );
    void clearTextures();
    bool windowShouldClose();
    void * getWindow();

    void rect( const Rect & rect, const Color & color );
    void sprite( unsigned int texture_id, unsigned int palette_id, const Rect & src, const Rect & dest, bool flip_x, bool flip_y );
    void character( const Character & character, const Color & color );
    void colorCanvas( const Color & color );
}

#endif // RENDER_H
#ifndef RENDER_H
#define RENDER_H

class Color;
class Rect;

namespace Render
{
    bool init( const char * title, int width, int height, Color background );
    void close();
    void startUpdate();
    void endUpdate();
    unsigned int getTextureId( const char * filename );
    void clearTextures();

    void rect( const Rect & rect, const Color & color );
    void sprite( int texture_id, const Rect & src, const Rect & dest );
}

#endif // RENDER_H
#ifndef RENDER_H
#define RENDER_H

class Color;
class RectGraphic;
class SpriteGraphic;

namespace Render
{
    bool init( const char * title, int width, int height, Color background );
    void close();
    void startUpdate();
    void endUpdate();
    unsigned int getTextureId( const char * filename );
    void clearTextures();

    void rect( const RectGraphic & graphic );
    void sprite( const SpriteGraphic & graphic );
}

#endif // RENDER_H
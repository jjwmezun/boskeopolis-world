#ifndef RENDERER_H
#define RENDERER_H

#include "arg.hpp"
#include "dir.hpp"
#include "nasringine/nasr.h"
#include "rect.hpp"
#include <vector>

namespace BSW
{
    class RectGraphic
    {
        public:
            unsigned int gfx_;

            inline float getX() const
            {
                return NasrGraphicsRectPaletteGetX( gfx_ );
            };
            inline void setX( float v ) const
            {
                NasrGraphicsRectPaletteSetX( gfx_, v );
            };
            inline float getY() const
            {
                return NasrGraphicsRectPaletteGetY( gfx_ );
            };
            inline void setY( float v ) const
            {
                NasrGraphicsRectPaletteSetY( gfx_, v );
            };
            inline void addToY( float v ) const
            {
                NasrGraphicsRectPaletteAddToY( gfx_, v );
            };
            inline void setH( float v ) const
            {
                NasrGraphicsRectPaletteSetH( gfx_, v );
            };
            inline float setOpacity() const
            {
                return NasrGraphicsRectPaletteGetOpacity( gfx_ );
            };
            inline void setOpacity( float v ) const
            {
                NasrGraphicsRectPaletteSetOpacity( gfx_, v );
            };
            inline void addToOpacity( float v ) const
            {
                NasrGraphicsRectPaletteAddToOpacity( gfx_, v );
            };
    };

    class SpriteGraphic
    {
        public:
            inline void setOpacity( float v ) const
            {
                NasrGraphicsSpriteSetOpacity( gfx_, v );
            };
            inline void setDestX( float v ) const
            {
                NasrGraphicsSpriteSetDestX( gfx_, v );
            };
            inline void setDestY( float v ) const
            {
                NasrGraphicsSpriteSetDestY( gfx_, v );
            };
            inline void setDestW( float v ) const
            {
                NasrGraphicsSpriteSetDestW( gfx_, v );
            };
            inline void setDestH( float v ) const
            {
                NasrGraphicsSpriteSetDestH( gfx_, v );
            };
            inline float getRotationX() const
            {
                return NasrGraphicsSpriteGetRotationX( gfx_ );
            };
            inline float getRotationY() const
            {
                return NasrGraphicsSpriteGetRotationY( gfx_ );
            };
            inline float getRotationZ() const
            {
                return NasrGraphicsSpriteGetRotationZ( gfx_ );
            };
            inline void setRotationX( float v ) const
            {
                NasrGraphicsSpriteSetRotationX( gfx_, v );
            };
            inline void setRotationY( float v ) const
            {
                NasrGraphicsSpriteSetRotationY( gfx_, v );
            };
            inline void setRotationZ( float v ) const
            {
                NasrGraphicsSpriteSetRotationZ( gfx_, v );
            };
            inline void setSrcX( float v ) const
            {
                NasrGraphicsSpriteSetSrcX( gfx_, v );
            };
            inline void setSrcY( float v ) const
            {
                NasrGraphicsSpriteSetSrcY( gfx_, v );
            };
            inline void setSrcW( float v ) const
            {
                NasrGraphicsSpriteSetSrcW( gfx_, v );
            };
            inline void setSrcH( float v ) const
            {
                NasrGraphicsSpriteSetSrcH( gfx_, v );
            };
            inline float getSrcX() const
            {
                return NasrGraphicsSpriteGetSrcX( gfx_ );
            };
            inline float getSrcY() const
            {
                return NasrGraphicsSpriteGetSrcY( gfx_ );
            };
            inline float getSrcW() const
            {
                return NasrGraphicsSpriteGetSrcW( gfx_ );
            };
            inline float getSrcH() const
            {
                return NasrGraphicsSpriteGetSrcH( gfx_ );
            };
            inline NasrRect getDest() const
            {
                return NasrGraphicsSpriteGetDest( gfx_ );
            };
            inline void setDest( const Rect & rect ) const
            {
                NasrRect r = { rect.x, rect.y, rect.w, rect.h };
                return NasrGraphicsSpriteSetDest( gfx_, r );
            };
            inline void flipX() const
            {
                NasrGraphicsSpriteFlipX( gfx_ );
            };
            inline void setFlipX( bool flip ) const
            {
                NasrGraphicsSpriteSetFlipX( gfx_, flip );
            };
            unsigned int gfx_;
    };

    class TextGraphic
    {
        public:
            unsigned int gfx_;

            inline void setOpacity( float v ) const
            {
                NasrSetTextOpacity( gfx_, v );
            };
            inline void setXOffset( float v ) const
            {
                NasrGraphicsTextSetXOffset( gfx_, v );
            };
            inline void setYOffset( float v ) const
            {
                NasrGraphicsTextSetYOffset( gfx_, v );
            };
    };

    struct CounterGraphic
    {
        unsigned int gfx_;

        inline void setNumber( float v ) const
        {
            NasrGraphicsCounterSetNumber( gfx_, v );
        }
    };

    class TilemapGraphics
    {
        public:
            constexpr TilemapGraphics( unsigned int gfx = 0 ) : gfx_ ( gfx ) {};
            inline void clearTile( unsigned int x, unsigned int y ) const
            {
                NasrGraphicsTilemapClearTile( gfx_, x, y );
            }

        private:
            unsigned int gfx_;
    };

    class Renderer
    {
        friend class Game;

        public:
            Renderer();

            unsigned int getTextureId( const std::string & texture ) const;

            SpriteGraphic addSprite
            (
                std::string texture,
                float srcx,
                float srcy,
                float w,
                float h,
                float x,
                float y,
                ArgList args = {}
            ) const;

            SpriteGraphic addTextureSprite
            (
                unsigned int texture,
                float srcx,
                float srcy,
                float w,
                float h,
                float x,
                float y,
                ArgList args = {}
            ) const;

            RectGraphic addRect
            (
                float x,
                float y,
                float w,
                float h,
                uint_fast8_t color,
                ArgList args = {}
            ) const;

            RectGraphic addScreen
            (
                uint_fast8_t color,
                ArgList args = {}
            ) const;

            unsigned int addRectGradient
            (
                float x,
                float y,
                float w,
                float h,
                Dir::XY dir,
                uint_fast8_t color1,
                uint_fast8_t color2,
                ArgList args = {}
            ) const;

            TilemapGraphics addTilemap
            (
                const std::string & tileset,
                const std::vector<NasrTile> & tiles,
                unsigned int width,
                unsigned int height,
                ArgList args = {}
            ) const;

            TextGraphic addText
            (
                ArgList args
            ) const;

            CounterGraphic addCounter
            (
                ArgList args
            ) const;

            SpriteGraphic addMenuBox
            (
                Text::Align align,
                Text::Valign valign,
                float w,
                float h,
                ArgList args = {}
            ) const;

            inline void setPalette( uint_fast8_t palette ) const
            {
                NasrSetGlobalPalette( palette );
            };

        private:
            unsigned int current_state_;
            unsigned int charset_;
    };
}

#endif // RENDERER_H
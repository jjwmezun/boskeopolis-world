#include "config.hpp"
#include "nasringine/nasr.h"
#include "renderer.hpp"
#include <stdexcept>

namespace BSW
{
    Renderer::Renderer()
    :
        current_state_ ( 0 ),
        charset_ ( 0 )
    {
        int charset = NasrAddCharset( "assets/graphics/charset/latin.png", "assets/charset/latin.json" );
        if ( charset < 0 )
        {
            throw std::runtime_error( "Missing charset file." );
        }
        charset_ = static_cast<unsigned int>( charset );
    };

    unsigned int Renderer::getTextureId( const std::string & texture ) const
    {
        std::string filename = "assets/graphics/" + texture;
        int texture_id = NasrLoadFileAsTexture( filename.c_str() );
        if ( texture_id < 0 )
        {
            throw std::runtime_error( "Invalid texture: " + texture );
        }

        return static_cast<unsigned int> ( texture_id );
    };

    SpriteGraphic Renderer::addSprite
    (
        std::string texture,
        float srcx,
        float srcy,
        float w,
        float h,
        float x,
        float y,
        ArgList args
    ) const
    {
        return addTextureSprite( getTextureId( texture ), srcx, srcy, w, h, x, y, args );
    };

    SpriteGraphic Renderer::addTextureSprite
    (
        unsigned int texture,
        float srcx,
        float srcy,
        float w,
        float h,
        float x,
        float y,
        ArgList args
    ) const
    {
        float scrollx = GetArg( "scrollx", args, 0.0f );
        float scrolly = GetArg( "scrolly", args, 0.0f );
        unsigned int layer = GetArgConvert<unsigned int, Layer> ( "layer", args, Layer::SPRITES_1 );
        uint_fast8_t flip_x = GetArgConvert<uint_fast8_t, bool> ( "flipx", args, false );
        uint_fast8_t flip_y = GetArgConvert<uint_fast8_t, bool> ( "flipy", args, false );
        float rotation_x = GetArg( "rotx", args, 0.0f );
        float rotation_y = GetArg( "roty", args, 0.0f );
        float rotation_z = GetArg( "rotz", args, 0.0f );
        float opacity = GetArg( "opacity", args, 1.0f );
        float tilingx = GetArg( "tilingx", args, 1.0f );
        float tilingy = GetArg( "tilingy", args, 1.0f );
        float srcw = GetArg( "srcw", args, w );
        float srch = GetArg( "srch", args, h );
        uint_fast8_t palette = GetArg( "palette", args, 0 );

        NasrRect src { srcx, srcy, srcw, srch };
        NasrRect dest { x, y, w, h };

        int graphic = NasrGraphicsAddSprite
        (
            scrollx,
            scrolly,
            current_state_,
            static_cast<unsigned int>( layer ),
            texture,
            src,
            dest,
            flip_x,
            flip_y,
            rotation_x,
            rotation_y,
            rotation_z,
            opacity,
            palette,
            0,
            tilingx,
            tilingy
        );

        if ( graphic < 0 )
        {
            throw std::runtime_error( "Invalid graphic for texture ID: " + std::to_string( texture ) );
        }

        return { static_cast<unsigned int>( graphic ) };
    };

    RectGraphic Renderer::addRect
    (
        float x,
        float y,
        float w,
        float h,
        uint_fast8_t color,
        float scrollx,
        float scrolly,
        Layer layer,
        float opacity
    ) const
    {
        NasrRect r { x, y, w, h };
        int graphic = NasrGraphicsAddRectPalette
        (
            scrollx,
            scrolly,
            current_state_,
            static_cast<unsigned int>( layer ),
            r,
            0,
            color,
            1,
            opacity
        );

        if ( graphic < 0 )
        {
            throw std::runtime_error( "Couldn’t generate rect graphic." );
        }

        return { static_cast<unsigned int>( graphic ) };
    };

    RectGraphic Renderer::addScreen
    (
        uint_fast8_t color,
        float scrollx,
        float scrolly,
        Layer layer,
        float opacity
    ) const
    {
        return addRect
        (
            0.0f,
            0.0f,
            static_cast<float> ( WINDOW_WIDTH_PIXELS ),
            static_cast<float> ( WINDOW_HEIGHT_PIXELS ),
            color,
            scrollx,
            scrolly,
            layer,
            opacity
        );
    };

    unsigned int Renderer::addRectGradient
    (
        float x,
        float y,
        float w,
        float h,
        Dir::XY dir,
        uint_fast8_t color1,
        uint_fast8_t color2,
        ArgList args
    ) const
    {
        float scrollx = GetArg( "scrollx", args, 0.0f );
        float scrolly = GetArg( "scrolly", args, 0.0f );
        float opacity = GetArg( "opacity", args, 1.0f );
        unsigned int layer = GetArgConvert<unsigned int, Layer> ( "layer", args, Layer::BG_1 );
        uint_fast8_t palette = GetArg( "palette", args, 0 );

        NasrRect r { x, y, w, h };
        int graphic = NasrGraphicsAddRectGradientPalette
        (
            scrollx,
            scrolly,
            current_state_,
            static_cast<unsigned int>( layer ),
            r,
            palette,
            static_cast<uint_fast8_t>( dir ),
            color1,
            color2,
            0,
            opacity
        );

        if ( graphic < 0 )
        {
            throw std::runtime_error( "Couldn’t generate gradient rect graphic." );
        }

        return static_cast<unsigned int>( graphic );
    };

    TilemapGraphics Renderer::addTilemap
    (
        const std::string & tileset,
        const std::vector<NasrTile> & tiles,
        unsigned int width,
        unsigned int height,
        ArgList args
    ) const
    {
        // Tileset.
        std::string tileset_file = "assets/graphics/tilesets/" + tileset + ".png";
        int tileset_id = NasrLoadFileAsTexture( tileset_file.c_str() );

        if ( tileset_id < 0 )
        {
            throw std::runtime_error( "Couldn’t generate tilemap graphic." );
        }

        float scrollx = GetArg( "scrollx", args, 0.0f );
        float scrolly = GetArg( "scrolly", args, 0.0f );
        unsigned int layer = static_cast<unsigned int>( GetArg( "layer", args, Layer::BLOCKS_1 ) );
        float opacity = GetArg( "opacity", args, 1.0f );
        float tilingx = GetArg( "tilingx", args, 1.0f );
        float tilingy = GetArg( "tilingy", args, 1.0f );

        int graphic = NasrGraphicsAddTilemap
        (
            scrollx,
            scrolly,
            current_state_,
            layer,
            tileset_id,
            &tiles[ 0 ],
            width,
            height,
            1,
            opacity,
            tilingx,
            tilingy
        );

        if ( graphic < 0 )
        {
            throw std::runtime_error( "Couldn’t generate tilemap graphic." );
        }

        return { static_cast<unsigned int>( graphic ) };
    };

    TextGraphic Renderer::addText( ArgList args ) const
    {
        std::string type = GetArg( "type", args, std::string( "normal" ) );
        std::string text = GetArg( "text", args, std::string( "" ) );
        float scrollx = GetArg( "scrollx", args, 1.0f );
        float scrolly = GetArg( "scrolly", args, 1.0f );
        unsigned int layer = GetArgConvert<unsigned int, Layer> ( "layer", args, Layer::AFTER_FG_2 );
        std::vector<char> cstring( text.c_str(), text.c_str() + text.size() + 1 );
        float x = GetArg( "x", args, 0.0f );
        float y = GetArg( "y", args, 0.0f );
        float w = GetArg( "w", args, static_cast<float>( WINDOW_WIDTH_PIXELS ) );
        float h = GetArg( "h", args, static_cast<float>( WINDOW_HEIGHT_PIXELS ) );
        uint_fast8_t palette = GetArg( "palette", args, 0 );
        NasrRect coords { x, y, w, h };
        uint_fast8_t align = GetArgConvert<uint_fast8_t, Text::Align> ( "align", args, NASR_ALIGN_DEFAULT );
        uint_fast8_t valign = GetArgConvert<uint_fast8_t, Text::Valign> ( "valign", args, NASR_VALIGN_DEFAULT );
        float padding_left = GetArg<float> ( "leftPadding", args, 0.0f );
        float padding_right = GetArg<float> ( "rightPadding", args, 0.0f );
        float padding_top = GetArg<float> ( "topPadding", args, 0.0f );
        float padding_bottom = GetArg<float> ( "bottomPadding", args, 0.0f );
        float xoffset = GetArg<float> ( "xOffset", args, 0.0f );
        float yoffset = GetArg<float> ( "yPadding", args, 0.0f );
        float shadow = GetArg<float> ( "shadow", args, 0.0f );
        float opacity = GetArg( "opacity", args, 1.0f );

        NasrText t
        {
            &cstring[ 0 ],
            charset_,
            coords,
            align,
            valign,
            padding_left,
            padding_right,
            padding_top,
            padding_bottom,
            xoffset,
            yoffset,
            shadow,
            opacity
        };

        int graphic = -1;
        if ( type == "gradient" )
        {
            uint_fast8_t dir = GetArgConvert<uint_fast8_t, Dir::XY>( "dir", args, Dir::XY::DOWN );
            uint_fast8_t color1 = GetArg( "color1", args, 1 );
            uint_fast8_t color2 = GetArg( "color2", args, 1 );
            graphic = NasrGraphicsAddTextGradientPalette
            (
                scrollx,
                scrolly,
                current_state_,
                layer,
                t,
                0,
                1,
                dir,
                color1,
                color2
            );
        }
        else
        {
            uint_fast8_t color = GetArg( "color", args, 1 );
            graphic = NasrGraphicsAddTextPalette
            (
                scrollx,
                scrolly,
                current_state_,
                layer,
                t,
                0,
                1,
                color
            );
        }

        if ( graphic < 0 )
        {
            throw std::runtime_error( "Couldn’t generate text graphic." );
        }

        return { static_cast<unsigned int>( graphic ) };
    };

    CounterGraphic Renderer::addCounter
    (
        ArgList args
    ) const
    {
        std::string type = GetArg( "type", args, std::string( "normal" ) );
        float num = GetArg( "num", args, 0.0f );
        unsigned int maxdigits = GetArg( "maxdigits", args, 0 );
        float x = GetArg( "x", args, 0.0f );
        float y = GetArg( "y", args, 0.0f );
        uint_fast8_t palette = GetArg( "palette", args, 0 );
        float scrollx = GetArg( "scrollx", args, 1.0f );
        float scrolly = GetArg( "scrolly", args, 1.0f );
        unsigned int layer = GetArgConvert<unsigned int, Layer> ( "layer", args, Layer::AFTER_FG_2 );
        float shadow = GetArg<float> ( "shadow", args, 0.0f );
        float opacity = GetArg( "opacity", args, 1.0f );
        unsigned int maxdecimals = GetArg( "maxdecimals", args, 0 );
        uint_fast8_t numpadding = GetArg( "numpadding", args, 0 );
        uint_fast8_t decimalpadding = GetArg( "decimalpadding", args, 0 );

        int graphic = -1;
        if ( type == "gradient" )
        {
            uint_fast8_t dir = GetArgConvert<uint_fast8_t, Dir::XY>( "dir", args, Dir::XY::DOWN );
            uint_fast8_t color1 = GetArg( "color1", args, 0 );
            uint_fast8_t color2 = GetArg( "color2", args, 0 );
            graphic = NasrGraphicsAddCounterPaletteGradient
            (
                scrollx,
                scrolly,
                current_state_,
                layer,
                charset_,
                num,
                maxdigits,
                maxdecimals,
                numpadding,
                decimalpadding,
                0,
                dir,
                color1,
                color2,
                1,
                x,
                y,
                shadow,
                opacity
            );
        }
        else
        {
            uint_fast8_t color = GetArg( "color", args, 0 );
            graphic = NasrGraphicsAddCounterPalette
            (
                scrollx,
                scrolly,
                current_state_,
                layer,
                charset_,
                num,
                maxdigits,
                maxdecimals,
                numpadding,
                decimalpadding,
                palette,
                color,
                1,
                x,
                y,
                shadow,
                opacity
            );
        }

        if ( graphic < 0 )
        {
            throw std::runtime_error( "Couldn’t generate counter graphic." );
        }

        return { static_cast<unsigned int>( graphic ) };
    };
    
    SpriteGraphic Renderer::addMenuBox
    (
        Text::Align align,
        Text::Valign valign,
        float w,
        float h,
        ArgList args
    ) const
    {
        int texture = NasrAddTextureBlank( static_cast<unsigned int>( w ), static_cast<unsigned int>( h ) );
        if ( texture < 0 )
        {
            throw std::runtime_error( "Couldn’t generate menu graphic." );
        }
        int menu_texture = NasrLoadFileAsTexture( "assets/graphics/misc/menu.png" );
        if ( menu_texture < 0 )
        {
            throw std::runtime_error( "Couldn’t generate menu graphic." );
        }

        NasrSetTextureAsTarget( texture );
        NasrRect bg { 1.0f, 1.0f, w - 3.0f, h - 3.0f };
        NasrDrawGradientRectToTexture(
            bg,
            NASR_DIR_DOWN,
            { 224.0f, 0.0f, 0.0f, 255.0f },
            { 255.0f, 0.0f, 0.0f, 255.0f }
        );

        // Draw top side
        NasrDrawSpriteToTexture
        (
            menu_texture,
            { 6.0f, 0.0f, 1.0f, 5.0f },
            { 6.0f, 0.0f, w - 12.0f, 5.0f },
            0,
            0,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            0,
            1,
            1.0f,
            1.0f
        );

        // Draw left side
        NasrDrawSpriteToTexture
        (
            menu_texture,
            { 0.0f, 6.0f, 5.0f, 1.0f },
            { 0.0f, 6.0f, 5.0f, h - 12.0f },
            0,
            0,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            0,
            1,
            1.0f,
            1.0f
        );

        // Draw right side
        NasrDrawSpriteToTexture
        (
            menu_texture,
            { 8.0f, 6.0f, 5.0f, 1.0f },
            { w - 5.0f, 6.0f, 5.0f, h - 12.0f },
            0,
            0,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            0,
            1,
            1.0f,
            1.0f
        );

        // Draw bottom side
        NasrDrawSpriteToTexture
        (
            menu_texture,
            { 6.0f, 8.0f, 1.0f, 5.0f },
            { 6.0f, h - 5.0f, w - 12.0f, 5.0f },
            0,
            0,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            0,
            1,
            1.0f,
            1.0f
        );

        // Draw top-left corner
        NasrDrawSpriteToTexture
        (
            menu_texture,
            { 0.0f, 0.0f, 6.0f, 6.0f },
            { 0.0f, 0.0f, 6.0f, 6.0f },
            0,
            0,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            0,
            1,
            1.0f,
            1.0f
        );

        // Draw top-right corner
        NasrDrawSpriteToTexture
        (
            menu_texture,
            { 7.0f, 0.0f, 6.0f, 6.0f },
            { w - 6.0f, 0.0f, 6.0f, 6.0f },
            0,
            0,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            0,
            1,
            1.0f,
            1.0f
        );

        // Draw bottom-left corner
        NasrDrawSpriteToTexture
        (
            menu_texture,
            { 0.0f, 7.0f, 6.0f, 6.0f },
            { 0.0f, h - 6.0f, 6.0f, 6.0f },
            0,
            0,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            0,
            1,
            1.0f,
            1.0f
        );

        // Draw bottom-right corner
        NasrDrawSpriteToTexture
        (
            menu_texture,
            { 7.0f, 7.0f, 6.0f, 6.0f },
            { w - 6.0f, h - 6.0f, 6.0f, 6.0f },
            0,
            0,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            0,
            1,
            1.0f,
            1.0f
        );
        NasrReleaseTextureTarget();

        float scrollx = GetArg( "scrollx", args, 1.0f );
        float scrolly = GetArg( "scrolly", args, 1.0f );
        unsigned int layer = GetArgConvert<unsigned int, Layer> ( "layer", args, Layer::AFTER_FG_2 );
        float opacity = GetArg( "opacity", args, 1.0f );
        float x = GetArg( "x", args, 0.0f );
        float y = GetArg( "y", args, 0.0f );

        int graphic = NasrGraphicsAddSprite
        (
            scrollx,
            scrolly,
            current_state_,
            static_cast<unsigned int>( layer ),
            texture,
            { 0.0f, 0.0f, w, h },
            {
                align == Text::Align::CENTER ? ( ( static_cast<float> ( WINDOW_WIDTH_PIXELS ) - w ) / 2.0f ) : x,
                valign == Text::Valign::MIDDLE ? ( ( static_cast<float> ( WINDOW_HEIGHT_PIXELS ) - h ) / 2.0f ) : y,
                w,
                h
            },
            0,
            0,
            0.0f,
            0.0f,
            0.0f,
            opacity,
            0,
            1,
            1.0f,
            1.0f
        );

        if ( graphic < 0 )
        {
            throw std::runtime_error( "Couldn’t generate menu graphic." );
        }

        return { static_cast<unsigned int>( graphic ) };
    };
}
#include "color.hpp"
#include "filename.hpp"
#include "rect.hpp"
#include "rect_graphic.hpp"
#include "render.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "sprite_graphic.hpp"
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace Render
{
    static SDL_Window * window;
    static SDL_Renderer * renderer;
    static SDL_Color background_color = { 0, 0, 0, 255 };
    static std::vector<SDL_Texture *> textures;
    static std::unordered_map<const char *, int> texture_map;

    static constexpr SDL_Color generateRawColor( Color color );
    static constexpr SDL_Rect generateRawRect( Rect rect );

    bool init( const char * title, int width, int height, Color background )
    {
        window = SDL_CreateWindow( title, 32, 32, width, height, SDL_WINDOW_RESIZABLE );
        if ( !window )
        {
            return false;
        }
        renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
        if ( !renderer )
        {
            return false;
        }

        background_color = generateRawColor( background );

        return true;
    };

    void close()
    {
        clearTextures();
        SDL_DestroyRenderer( renderer );
        SDL_DestroyWindow( window );
    };

    void startUpdate()
    {
        SDL_SetRenderDrawColor( renderer, background_color.r, background_color.g, background_color.b, background_color.a );
        SDL_RenderClear( renderer );
    };

    void endUpdate()
    {
        SDL_RenderPresent( renderer );
    };

    void clearTextures()
    {
        for ( auto * texture : textures )
        {
            SDL_DestroyTexture( texture );
        }
        textures.clear();
        texture_map.clear();
    };

    unsigned int getTextureId( const char * filename )
    {
        const auto iterator = texture_map.find( filename );
        if ( iterator != texture_map.end() )
        {
            return iterator->second;
        }

        const std::string full_filename_str = Filename::image( filename );
        const char * full_filename = full_filename_str.c_str();
        SDL_RWops * rwop = SDL_RWFromFile( full_filename, "rb" );
        if ( !rwop )
        {
            SDL_Log( "SDL_RWFromFile failed: %s", SDL_GetError() );
            throw std::runtime_error( "Could not load file " + std::string( filename ) );
        }
        SDL_Surface * surface = IMG_LoadPNG_RW( rwop );
        SDL_RWclose( rwop );
        if ( !surface )
        {
            SDL_Log( "SDL_Surface failed: %s", SDL_GetError() );
            throw std::runtime_error( "Could not load surface for " + std::string( filename ) );
        }
        SDL_Texture * texture = SDL_CreateTextureFromSurface( renderer, surface );
        SDL_FreeSurface( surface );
        if ( !texture )
        {
            SDL_Log( "SDL_CreateTextureFromSurface failed: %s", SDL_GetError() );
            throw std::runtime_error( "Could not generate texture for " + std::string( filename ) );
        }
        textures.emplace_back( texture );
        texture_map.insert( std::pair<const char *, int> { filename, textures.size() - 1 } );
        return textures.size() - 1;
    };

    void rect( const RectGraphic & graphic )
    {
        SDL_Color raw_color = generateRawColor( graphic.color );
        SDL_Rect raw_rect = generateRawRect( graphic.rect );
        SDL_SetRenderDrawColor( renderer, raw_color.r, raw_color.g, raw_color.b, raw_color.a );
        SDL_RenderFillRect( renderer, &raw_rect );
    };

    void sprite( const SpriteGraphic & graphic )
    {
        const auto src = generateRawRect( graphic.src );
        const auto dest = generateRawRect( graphic.dest );
        SDL_RenderCopyEx
        (
            renderer,
            textures[ graphic.texture_id ],
            &src,
            &dest,
            0.0,
            nullptr,
            SDL_FLIP_NONE
        );
    };

    static constexpr SDL_Color generateRawColor( Color color )
    {
        return
        {
            ( Uint8 )( color.r ),
            ( Uint8 )( color.g ),
            ( Uint8 )( color.b ),
            ( Uint8 )( color.a )
        };
    };

    static constexpr SDL_Rect generateRawRect( Rect rect )
    {
        return
        {
            ( int )( rect.x ),
            ( int )( rect.y ),
            ( int )( rect.w ),
            ( int )( rect.h )
        };
    };
}
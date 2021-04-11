#include "character.hpp"
#include "color.hpp"
#include "filename.hpp"
#include "rect.hpp"
#include "render.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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
    static SDL_Surface * text_surface = nullptr;
    static SDL_Texture * text_texture = nullptr;
    static SDL_Color text_color = { 0, 0, 0, 255 };

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

        const std::string full_filename_str = Filename::image( "text/latin.png" );
        const char * full_filename = full_filename_str.c_str();
        SDL_RWops * rwop = SDL_RWFromFile( full_filename, "rb" );
        if ( !rwop )
        {
            SDL_Log( "SDL_RWFromFile failed: %s", SDL_GetError() );
            throw std::runtime_error( "Could not load graphic file for text" );
        }
        text_surface = IMG_LoadPNG_RW( rwop );
        SDL_RWclose( rwop );
        if ( !text_surface )
        {
            SDL_Log( "SDL_Surface failed: %s", SDL_GetError() );
            throw std::runtime_error( "Could not load graphic surface for text" );
        }
        text_texture = SDL_CreateTextureFromSurface( renderer, text_surface );
        if ( !text_texture )
        {
            SDL_Log( "SDL_CreateTextureFromSurface failed: %s", SDL_GetError() );
            throw std::runtime_error( "Could not generate texture for text" );
        }

        return true;
    };

    void close()
    {
        SDL_DestroyTexture( text_texture );
        SDL_FreeSurface( text_surface );
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

    void rect( const Rect & rect, const Color & color )
    {
        SDL_Color raw_color = generateRawColor( color );
        SDL_Rect raw_rect = generateRawRect( rect );
        SDL_SetRenderDrawColor( renderer, raw_color.r, raw_color.g, raw_color.b, raw_color.a );
        SDL_RenderFillRect( renderer, &raw_rect );
    };

    void sprite( int texture_id, const Rect & src, const Rect & dest )
    {
        const auto raw_src = generateRawRect( src );
        const auto raw_dest = generateRawRect( dest );
        SDL_RenderCopyEx
        (
            renderer,
            textures[ texture_id ],
            &raw_src,
            &raw_dest,
            0.0,
            nullptr,
            SDL_FLIP_NONE
        );
    };

    void character( const Character & character )
    {
        /*
        SDL_Color color = generateRawColor( character.color );
        if ( color.r != text_color.r || color.g != text_color.g || color.b != text_color.b || color.a != text_color.a )
        {
            text_color = color;
            SDL_LockSurface( text_surface );
            Uint32 * pixels = ( Uint32 * )( text_surface->pixels );
            Uint32 pixel_color = SDL_MapRGBA( text_surface->format, color.r, color.g, color.b, color.a );
            for ( int i = 0; i < ( text_surface->h * text_surface->pitch ) / text_surface->format->BytesPerPixel; ++i )
            {
                Uint32 a = pixels[ i ] & text_surface->format->Amask;
                a = a >> text_surface->format->Ashift;
                a = a << text_surface->format->Aloss;
                if ( a != 0 )
                {
                    pixels[ i ] = pixel_color;
                }
            }
            SDL_UnlockSurface( text_surface );
            SDL_Texture * temp = SDL_CreateTextureFromSurface( renderer, text_surface );
            if ( !temp )
            {
                SDL_Log( "SDL_CreateTextureFromSurface failed: %s", SDL_GetError() );
                throw std::runtime_error( "Error colorizing text." );
            }
            else
            {
                SDL_DestroyTexture( text_texture );
                text_texture = temp;
            }
        }*/

        const auto raw_src = generateRawRect( character.src );
        const auto raw_dest = generateRawRect( character.dest );
        SDL_RenderCopyEx
        (
            renderer,
            text_texture,
            &raw_src,
            &raw_dest,
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
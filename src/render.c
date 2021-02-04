#include "assert.h"
#include "asset.h"
#include "color.h"
#include "config.h"
#include "layers.h"
#include "graphics.h"
#include "render.h"
#include "rect.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define MAX_TEXTURES 255
#define MAX_TEXTURE_STRING 32
#define MAX_GRAPHICS 64

static graphics_t graphics[ MAX_GRAPHICS ];
static int graphics_ids[ MAX_GRAPHICS ];
static char texture_names[ MAX_TEXTURES ][ MAX_TEXTURE_STRING ];
static SDL_Texture * textures[ MAX_TEXTURES ];
static color_t background_color = { 0, 0, 0, 255 };
static SDL_Window * window;
static SDL_Renderer * renderer;
static SDL_Texture * canvas;
static SDL_Texture * master_texture;
static int number_of_graphics = 0;
static int number_of_textures = 0;

void render_execute()
{
    SDL_SetRenderDrawColor( renderer, background_color.r, background_color.g, background_color.b, background_color.a );
    SDL_RenderClear( renderer );
    for ( int i = 0; i < number_of_graphics; ++i )
    {
        switch ( graphics[ i ].type )
        {
            case ( GRAPHICS_REGULAR ):
            {
                render_sprite( &graphics[ i ].data.regular );
            }
            break;
        }
    }

    SDL_SetRenderTarget( renderer, canvas );
    SDL_RenderClear( renderer );
    const SDL_Rect src = { 0, 0, WINDOW_WIDTH_PIXELS, WINDOW_HEIGHT_PIXELS };
    const SDL_Rect dest = { 0, 0, WINDOW_WIDTH_PIXELS * 4, WINDOW_HEIGHT_PIXELS * 4 };
    if ( SDL_RenderCopy( renderer, master_texture, &src, &dest ) != 0 )
    {
        SDL_Log( "Render failure: %s\n", SDL_GetError() );
    }
    SDL_RenderPresent( renderer );
    SDL_SetRenderTarget( renderer, master_texture );
};

int render_init( const char * title, int width, int height )
{
    window = SDL_CreateWindow
    (
        title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width * 4,
		height * 4,
		SDL_WINDOW_RESIZABLE
    );
    if ( window == NULL )
    {
        SDL_Log( "Unable to create window: %s", SDL_GetError() );
        return 1;
    }
    renderer = SDL_CreateRenderer( window, -1, 0 );
    if ( renderer == NULL )
    {
        SDL_Log( "Unable to create renderer: %s", SDL_GetError() );
        return 1;
    }

    canvas = SDL_GetRenderTarget( renderer );
    SDL_SetTextureBlendMode( canvas, SDL_BLENDMODE_BLEND );
    master_texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH_PIXELS * 4, WINDOW_HEIGHT_PIXELS * 4 );
    SDL_SetTextureBlendMode( master_texture, SDL_BLENDMODE_BLEND );
    SDL_SetRenderTarget( renderer, master_texture );

    return 0;
};

void render_sprite( const struct graphics_data_regular_t * data )
{
    SDL_Rect raw_src = { data->src.x, data->src.y, data->src.w, data->src.h };
    SDL_Rect raw_dest = { data->dest.x, data->dest.y, data->dest.w, data->dest.h };
    if ( SDL_RenderCopyEx( renderer, textures[ data->texture ], &raw_src, &raw_dest, data->rotation, NULL, SDL_FLIP_NONE ) != 0 )
    {
        SDL_Log( "Render failure: %s\n", SDL_GetError() );
    }
};

void render_rect( const struct rect_t * rect, const struct color_t * color )
{
    SDL_Rect raw_rect = { rect->x, rect->y, rect->w, rect->h };
    SDL_SetRenderDrawColor( renderer, color->r, color->g, color->b, color->a );
    SDL_RenderFillRect( renderer, &raw_rect );
};

void render_close()
{
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
};

int render_get_texture_id( const char * filename )
{
    // Look for texture ID.
    for ( int i = 0; i < number_of_textures; ++i )
    {
        if ( strncmp( texture_names[ i ], filename, MAX_TEXTURE_STRING ) == 0 )
        {
            return i;
        }
    }

    // If not texture not found, load it.
    char * full_filename = asset_image( filename );
    SDL_RWops * rwop = SDL_RWFromFile( full_filename, "rb" );
    free( full_filename );
    if ( !rwop )
    {
        SDL_Log( "SDL_RWFromFile failed: %s", SDL_GetError() );
        return -1;
    }
    SDL_Surface * surface = IMG_LoadPNG_RW( rwop );
    SDL_RWclose( rwop );
    if ( !surface )
    {
        SDL_Log( "SDL_Surface failed: %s", SDL_GetError() );
        return -1;
    }
    SDL_Texture * texture = SDL_CreateTextureFromSurface( renderer, surface );
    SDL_FreeSurface( surface );
    if ( !texture )
    {
        SDL_Log( "SDL_CreateTextureFromSurface failed: %s", SDL_GetError() );
        return -1;
    }

    // Set next empty space for texture & texture names & return that space’s ID.
    textures[ number_of_textures ] = texture;
    strncpy( texture_names[ number_of_textures ], filename, MAX_TEXTURE_STRING );
    return number_of_textures++;
};

struct graphics_t * render_get_graphics( int id )
{
    assert( id > 0 );
    return &graphics[ graphics_ids[ id - 1 ] ];
};

int render_add_graphics( const struct graphics_t * gfx )
{
    assert( number_of_graphics < MAX_GRAPHICS );
    ++number_of_graphics;
    int i = number_of_graphics - 2;
    while ( i >= 0 && gfx->layer < graphics[ i ].layer )
    {
        memcpy( &graphics[ i + 1 ], &graphics[ i ], sizeof( graphics_t ) );
        for ( int j = 0; j < number_of_graphics; ++j )
        {
            if ( graphics_ids[ j ] == i )
            {
                ++graphics_ids[ j ];
            }
        }
        --i;
    }
    memcpy( &graphics[ i + 1 ], gfx, sizeof( graphics_t ) );
    graphics_ids[ number_of_graphics - 1 ] = i + 1;
    return number_of_graphics;
};

int render_create_custom_texture( const char * name, int width, int height )
{
    SDL_Texture * texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height );
    if ( !texture )
    {
        SDL_Log( "SDL_CreateTexture failed: %s", SDL_GetError() );
        return -1;
    }
    textures[ number_of_textures ] = texture;
    strncpy( texture_names[ number_of_textures ], name, MAX_TEXTURE_STRING );
    SDL_SetTextureBlendMode( texture, SDL_BLENDMODE_BLEND );
    return number_of_textures++;
};

void render_set_target_texture( int id )
{
    SDL_SetRenderTarget( renderer, textures[ id ] );
};

void render_release_target_texture()
{
    SDL_SetRenderTarget( renderer, master_texture );
};

void render_clear()
{
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 0 );
    SDL_RenderClear( renderer );
}
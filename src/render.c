#include "assert.h"
#include "asset.h"
#include "character.h"
#include "color.h"
#include "config.h"
#include "layers.h"
#include "game_state.h"
#include "graphics.h"
#include "render.h"
#include "rect.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "text.h"

#define MAX_TEXTURES 255
#define MAX_TEXTURE_STRING 32
#define MAX_GRAPHICS 64

static graphics_t graphics[ MAX_STATES ][ MAX_GRAPHICS ];
static int graphics_ids[ MAX_STATES ][ MAX_GRAPHICS ];
static char texture_names[ MAX_TEXTURES ][ MAX_TEXTURE_STRING ];
static color_t background_color = { 0, 0, 0, 255 };
static SDL_Rect canvas_rect = { 0, 0, WINDOW_WIDTH_PIXELS, WINDOW_HEIGHT_PIXELS };
static SDL_Texture * textures[ MAX_TEXTURES ];
static SDL_Window * window;
static SDL_Renderer * renderer;
static SDL_Texture * canvas;
static SDL_Texture * master_texture;
static SDL_Texture * current_target;
static int number_of_graphics[ MAX_STATES ];
static int number_of_textures = 0;
static SDL_Surface * text_surface = NULL;
static SDL_Texture * text_texture = NULL;
static color_t text_color = { 0, 0, 0, 255 };

static int render_find_texture_id( const char * filename );

void render_execute()
{
    SDL_SetRenderDrawColor( renderer, background_color.r, background_color.g, background_color.b, background_color.a );
    SDL_RenderClear( renderer );
    int last_state = game_state_current_index();
    for ( int state = 0; state <= last_state; ++state )
    {
        for ( int i = 0; i < number_of_graphics[ state ]; ++i )
        {
            switch ( graphics[ state ][ i ].type )
            {
                case ( GRAPHICS_REGULAR ):
                {
                    render_sprite( &graphics[ state ][ i ].data.regular );
                }
                break;
                case ( GRAPHICS_RECT ):
                {
                    render_rect( &graphics[ state ][ i ].data.rect.dest, &graphics[ state ][ i ].data.rect.color );
                }
                break;
            }
        }
    }

    SDL_SetRenderTarget( renderer, canvas );
    SDL_RenderClear( renderer );
    const SDL_Rect dest = { 0, 0, WINDOW_WIDTH_PIXELS * 4, WINDOW_HEIGHT_PIXELS * 4 };
    if ( SDL_RenderCopy( renderer, master_texture, &canvas_rect, &dest ) != 0 )
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
    current_target = master_texture;

    // If not texture not found, load it.
    char * filename = asset_image( "text/latin.png" );
    SDL_RWops * rwop = SDL_RWFromFile( filename, "rb" );
    free( filename );
    if ( !rwop )
    {
        SDL_Log( "SDL_RWFromFile failed: %s", SDL_GetError() );
        return -1;
    }
    text_surface = IMG_LoadPNG_RW( rwop );
    SDL_RWclose( rwop );
    if ( !text_surface )
    {
        SDL_Log( "SDL_Surface failed: %s", SDL_GetError() );
        return -1;
    }
    text_texture = SDL_CreateTextureFromSurface( renderer, text_surface );
    if ( !text_texture )
    {
        SDL_Log( "SDL_CreateTextureFromSurface failed: %s", SDL_GetError() );
        return -1;
    }

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

void render_text( const struct text_t * text )
{
    for ( int i = 0; i < text->count; ++i )
    {
        render_character( &text->characters[ i ] );
    }
};

void render_character( const struct character_t * character )
{
    if ( character->color.r != text_color.r || character->color.g != text_color.g || character->color.b != text_color.b || character->color.a != text_color.a )
    {
        text_color = character->color;
        SDL_LockSurface( text_surface );
        Uint32 * pixels = ( Uint32 * )( text_surface->pixels );
        Uint32 pixel_color = SDL_MapRGBA( text_surface->format, character->color.r, character->color.g, character->color.b, character->color.a );
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
        }
        else
        {
            SDL_DestroyTexture( text_texture );
            text_texture = temp;
        }
    }
    SDL_Rect raw_src = { character->src.x, character->src.y, character->src.w, character->src.h };
    SDL_Rect raw_dest = { character->dest.x, character->dest.y, character->dest.w, character->dest.h };
    if ( SDL_RenderCopyEx( renderer, text_texture, &raw_src, &raw_dest, 0.0, NULL, SDL_FLIP_NONE ) != 0 )
    {
        SDL_Log( "Render failure: %s\n", SDL_GetError() );
    }
};

void render_close()
{
    SDL_FreeSurface( text_surface );
    SDL_DestroyTexture( text_texture );
    for ( int i = 0; i < number_of_textures; ++i )
    {
        if ( textures[ i ] )
        {
            SDL_DestroyTexture( textures[ i ] );
        }
    }
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
};

int render_get_texture_id( const char * filename )
{
    // Look for texture ID.
    render_find_texture_id( filename );

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
    int state = game_state_current_index();
    assert( id > 0 );
    return &graphics[ state ][ graphics_ids[ state ][ id - 1 ] ];
};

int render_add_graphics( const struct graphics_t * gfx )
{
    int state = game_state_current_index();
    assert( number_of_graphics[ state ] < MAX_GRAPHICS );
    ++number_of_graphics[ state ];
    int i = number_of_graphics[ state ] - 2;
    while ( i >= 0 && gfx->layer < graphics[ state ][ i ].layer )
    {
        memcpy( &graphics[ state ][ i + 1 ], &graphics[ state ][ i ], sizeof( graphics_t ) );
        for ( int j = 0; j < number_of_graphics[ state ]; ++j )
        {
            if ( graphics_ids[ state ][ j ] == i )
            {
                ++graphics_ids[ state ][ j ];
            }
        }
        --i;
    }
    memcpy( &graphics[ state ][ i + 1 ], gfx, sizeof( graphics_t ) );
    graphics_ids[ state ][ number_of_graphics[ state ] - 1 ] = i + 1;
    return number_of_graphics[ state ];
};

void render_remove_graphics( int id )
{
    int state = game_state_current_index();
    if ( id > 0 )
    {
        graphics[ state ][ graphics_ids[ state ][ id - 1 ] ].type = GRAPHICS_NULL;
    }
};

int render_create_custom_texture( const char * name, int width, int height )
{
    int texture_id = render_find_texture_id( name );
    if ( texture_id >= 0 )
    {
        return texture_id;
    }
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
    current_target = textures[ id ];
    SDL_SetRenderTarget( renderer, textures[ id ] );
};

void render_release_target_texture()
{
    current_target = master_texture;
    SDL_SetRenderTarget( renderer, master_texture );
};

void render_clear()
{
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 0 );
    SDL_RenderClear( renderer );
};

void render_color_canvas( const struct color_t * color )
{
    SDL_SetRenderDrawColor( renderer, color->r, color->g, color->b, color->a );
    SDL_RenderFillRect( renderer, &canvas_rect );
};

void render_clear_graphics()
{
    number_of_graphics[ game_state_current_index() ] = 0;
};

void render_clear_textures()
{
    for ( int i = 0; i < MAX_STATES; ++i )
    {
        number_of_graphics[ i ] = 0;
    }
    for ( int i = 0; i < number_of_textures; ++i )
    {
        if ( textures[ i ] )
        {
            SDL_DestroyTexture( textures[ i ] );
        }
    }
    number_of_textures = 0;
};

static int render_find_texture_id( const char * filename )
{
    for ( int i = 0; i < number_of_textures; ++i )
    {
        if ( strncmp( texture_names[ i ], filename, MAX_TEXTURE_STRING ) == 0 )
        {
            return i;
        }
    }
    return -1;
};
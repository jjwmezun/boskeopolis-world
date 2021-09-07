#include "color.hpp"
#include "engine.hpp"
#include "layer.hpp"
#include "localization.hpp"
#include "log.hpp"
#include "rect.hpp"
#include "render.hpp"

#include <cstdlib>
#include "text.hpp"

static constexpr int DT = 17;

static int running = 1;
static int ticks = 0;
static int accumulator = 0;

int main()
{
    if ( !Engine::init() )
    {
        Log::sendError( "¡Error! Failed to initialize game!" );
        return -1;
    }
    Localization::init();

    Color c;
    c.r = 255.0f;
    c.g = 255.0f;
    c.b = 255.0f;
    c.a = 255.0f;
    Render::addGraphic( Graphic::createFullRect( c ), 1, Layer::BEFORE_BLOCKS_1 );
    int * tiles = ( int * )( calloc( 25 * 14, sizeof( int ) ) );
    tiles[ 5 ] = 4102;
    Render::addTilemap( "urban", tiles, 25, 14, 2, 1, Layer::BLOCKS_1 );

    Graphic sprite = Graphic::createSprite( Render::getTextureID( "sprites/autumn.png" ), 0, { 0, 0, 16, 24 }, 0, 0 );
    Render::addGraphic( sprite, 1, Layer::SPRITES_1 );

    Text t { "There have been several rumors, misconceptions, and urban legends regarding production history, fictional details, and gameplay elements within the Mario franchise. This article is a list of the history of Mario rumors and details the extent to which they are true. Each section is supported by one or more references that detail the spread or debunking of their respective topics. While the origins of some rumors remain a mystery, others stem from unsourced edits on articles from Wikipedia, or from the Super Mario Wiki. Others have been spread through misinterpretation, or gags being received and spread as fact. The above text is from the Super Mario Wiki and is available under a Creative Commons license. Attribution must be provided through a list of authors or a link back to the original article. Source: https://www.mariowiki.com/Main_Page", { { "x_padding", 8.0f }, { "y_padding", 8.0f }, { "color", Color{ 255.0f, 0.0f, 0.0f, 255.0f } } } };
    Graphic text = Graphic::createText( t );
    const unsigned int tt = Render::addGraphic( text, 1, Layer::FG_1 );

    ticks = Engine::getTicks();
    while ( running )
    {
        running = Engine::handleEvents();
        int new_time = Engine::getTicks();
        int frame_time = new_time - ticks;
        ticks = new_time;
        accumulator += frame_time;
        while ( accumulator >= DT )
        {
            accumulator -= DT;
        }

        if ( ticks > 5000 )
        {
            Render::removeGraphic( tt );
        }

        Render::update();
    }

    free( tiles );

    Render::close();
    Engine::close();
    return 0;
}
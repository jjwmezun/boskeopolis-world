#include "color.hpp"
#include "engine.hpp"
#include "layer.hpp"
#include "localization.hpp"
#include "log.hpp"
#include "rect.hpp"
#include "render.hpp"

#include <cstdlib>

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

        Render::update();
    }

    free( tiles );

    Render::close();
    Engine::close();
    return 0;
}
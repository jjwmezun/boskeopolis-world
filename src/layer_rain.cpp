#include "graphic.hpp"
#include "layer_rain.hpp"
#include "math.hpp"
#include "render.hpp"

float randSpeed()
{
    return ( double )( Math::randInt( 50, 800 ) ) / 100.0;
};

float randHeight()
{
    return Math::randInt( 32, 128 );
};

void LayerRain::init( unsigned int state )
{
    for ( int i = 0; i < NUMBER_OF_RAINDROPS; ++i )
    {
        raindrops[ i ].graphic = Render::addGraphic( Graphic::createRect
        (
            { ( float )( i * 2 + 1 ), ( float )( Math::randInt( -128, 224 ) ), 1, randHeight() },
            { 48.0f, 52.0f, 64.0f, 80.0f },
            true
        ), state, Layer::FG_2 );
        raindrops[ i ].speed = randSpeed();
    }
};

void LayerRain::update()
{
    for ( int i = 0; i < NUMBER_OF_RAINDROPS; ++i )
    {
        RectGraphics & r = std::get<RectGraphics> ( Render::getGraphic( raindrops[ i ].graphic )->data );
        r.rect.y += raindrops[ i ].speed;
        if ( r.rect.y > ( float )( Unit::WINDOW_HEIGHT_PIXELS ) )
        {
            r.rect.h = randHeight();
            r.rect.y = -r.rect.h - Math::randInt( 8, 128 );
            raindrops[ i ].speed = randSpeed();
        }
    }
};
#include "state_graphics.hpp"

void StateGraphics::init()
{
    for ( int i = 0; i < Unit::NUMBER_OF_LAYERS; ++i )
    {
        graphics_per_layer_[ i ] = 0;
    }
};

void StateGraphics::render()
{
    for ( int i = 0; i < Unit::NUMBER_OF_LAYERS; ++i )
    {
        for ( int j = 0; j < graphics_per_layer_[ i ]; ++j )
        {
            layers_[ i ][ j ].render();
        }
    }
};

void StateGraphics::addGraphic( Graphics g, Unit::Layer layer )
{
    int i = ( int )( layer );
    layers_[ i ][ graphics_per_layer_[ i ]++ ] = g;
};
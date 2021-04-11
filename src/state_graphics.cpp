#include "state_graphics.hpp"
#include "rect.hpp"
#include <stdexcept>
#include <vector>

void StateGraphics::init()
{
    for ( int i = 0; i < Unit::NUMBER_OF_LAYERS; ++i )
    {
        number_of_graphics_per_layer_[ i ] = 0;
    }
};

int StateGraphics::addGraphic( Graphic graphic, Unit::Layer layer )
{
    if ( number_of_graphics_per_layer_[ ( int )( layer ) ] == MAX_GRAPHICS_PER_LAYER - 1 )
    {
        throw std::runtime_error( "Too many graphics added to layer." );
        return -1;
    }
    graphic.setId( latest_id_ );
    layers_[ ( int )( layer ) ][ number_of_graphics_per_layer_[ ( int )( layer ) ] ] = graphic;
    graphics_map_.insert( std::pair<int, Graphic *>{ latest_id_, &layers_[ ( int )( layer ) ][ number_of_graphics_per_layer_[ ( int )( layer ) ] ] } );
    ++number_of_graphics_per_layer_[ ( int )( layer ) ];
    return latest_id_++;
};

RectGraphic & StateGraphics::getRect( int id )
{
    auto iterator = graphics_map_.find( id );
    if ( iterator == graphics_map_.end() || iterator->second == nullptr )
    {
        throw std::runtime_error( "Could not find graphic." );
    }
    return iterator->second->getRect();
};

void StateGraphics::render() const
{
    for ( int layer = 0; layer < Unit::NUMBER_OF_LAYERS; ++layer )
    {
        for ( int i = 0; i < number_of_graphics_per_layer_[ layer ]; ++i )
        {
            layers_[ layer ][ i ].render();
        }
    }
};
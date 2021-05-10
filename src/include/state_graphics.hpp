#ifndef STATE_GRAPHICS_H
#define STATE_GRAPHICS_H

#include "graphics.hpp"
#include "unit.hpp"

static constexpr int MAX_GRAPHICS_PER_LAYER = 64;

class StateGraphics
{
    public:
        void init();
        void render();
        void addGraphic( Graphics g, Unit::Layer layer );

    private:
        Graphics layers_[ Unit::NUMBER_OF_LAYERS ][ MAX_GRAPHICS_PER_LAYER ];
        int graphics_per_layer_[ Unit::NUMBER_OF_LAYERS ];
};

#endif // STATE_GRAPHICS_H
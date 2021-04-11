#ifndef STATE_GRAPHICS_H
#define STATE_GRAPHICS_H

#include "graphic.hpp"
#include "unit.hpp"
#include <unordered_map>

class RectGraphic;

class StateGraphics
{
    public:
        void init();
        int addGraphic( Graphic graphic, Unit::Layer layer );
        RectGraphic & getRect( int id );
        void render() const;

    private:
        static constexpr int MAX_GRAPHICS_PER_LAYER = 50;

        int latest_id_ = 0;
        Graphic layers_[ Unit::NUMBER_OF_LAYERS ][ MAX_GRAPHICS_PER_LAYER ];
        int number_of_graphics_per_layer_[ Unit::NUMBER_OF_LAYERS ];
        std::unordered_map<int, Graphic *> graphics_map_;
};

#endif // STATE_GRAPHICS_H
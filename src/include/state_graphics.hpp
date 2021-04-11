#ifndef STATE_GRAPHICS_H
#define STATE_GRAPHICS_H

#include "graphic.hpp"
#include <memory>
#include "unit.hpp"
#include <unordered_map>

class StateGraphics
{
    public:
        StateGraphics();
        int addGraphic( Graphic * graphic, Unit::Layer layer );
        void render() const;

    private:
        static constexpr int MAX_GRAPHICS_PER_LAYER = 50;

        int latest_id_ = 0;
        std::unique_ptr<Graphic> layers_[ Unit::NUMBER_OF_LAYERS ][ MAX_GRAPHICS_PER_LAYER ];
        int number_of_graphics_per_layer_[ Unit::NUMBER_OF_LAYERS ];
        std::unordered_map<int, Graphic *> graphics_map_;
};

#endif // STATE_GRAPHICS_H
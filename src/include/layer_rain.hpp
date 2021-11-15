#ifndef LAYER_RAIN_H
#define LAYER_RAIN_H

#include <cmath>
#include "unit.hpp"

class LayerRain
{
    public:
        void init( unsigned int state );
        void update();

    private:
        struct RainDrop
        {
            unsigned int graphic;
            float speed;
        };
        static constexpr int NUMBER_OF_RAINDROPS = ( int )( std::floor( ( double )( Unit::WINDOW_WIDTH_PIXELS ) / 2.0 ) );
        RainDrop raindrops[ NUMBER_OF_RAINDROPS ];
};

#endif // LAYER_RAIN_H
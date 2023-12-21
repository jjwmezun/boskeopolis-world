#ifndef INVENTORY_H
#define INVENTORY_H

#include "renderer.hpp"

namespace BSW
{
    class Game;

    struct Inventory
    {
        void init( const Game & game );
        void updateMoney( float n );

        float money_;
        CounterGraphic money_gfx_;        
    };
}

#endif // INVENTORY_H
#ifndef TREASURE_LIST_H
#define TREASURE_LIST_H

#include "inventory.hpp"

class TreasureListState
{
    public:
        TreasureListState( Inventory _inventory );
        void update();
        void init( unsigned int state );

    private:
        bool leaving = false;
        unsigned int bg = -1;
        float opacity = 0.0f;
        Inventory inventory;

        void updateBGOpacity();
};

#endif // TREASURE_LIST_H
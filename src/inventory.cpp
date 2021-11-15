#include "inventory.hpp"

#include <cstdio>

Inventory::Inventory()
:
    gems ( 0 ),
    shown_gems ( 0 )
{
    for ( int i = 0; i < MAX_TREASURES; ++i )
    {
        treasures_collected[ i ] = false;
    }
};

void Inventory::update()
{
    if ( shown_gems < gems - 25 )
    {
        shown_gems += 25;
    }
    else if ( shown_gems < gems )
    {
        shown_gems = gems;
    }
};

void Inventory::addGems( int amount )
{
    gems += amount;
};

int Inventory::getShownGems() const
{
    return shown_gems;
};

void Inventory::getTreasure( int id )
{
    treasures_collected[ id ] = true;
};

bool Inventory::hasTreasure( TreasureType type ) const
{
    return treasures_collected[ ( int )( type ) ];
};
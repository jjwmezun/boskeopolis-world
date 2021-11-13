#include "inventory.hpp"

Inventory::Inventory()
:
    gems ( 0 ),
    shown_gems ( 0 )
{};

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
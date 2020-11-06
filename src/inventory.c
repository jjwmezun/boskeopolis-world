#include "inventory.h"

#include <stdio.h>

#define GEMS_DISPLAY_INCREMENT 25

static int gems = 0;
static int gems_display = 0;

void inventory_update()
{
    // Update Gems Display.
    if ( gems > gems_display )
    {
        gems_display += GEMS_DISPLAY_INCREMENT;
        if ( gems_display > gems )
        {
            gems_display = gems;
        }
    }
    else if ( gems < gems_display )
    {
        gems_display -= GEMS_DISPLAY_INCREMENT;
        if ( gems_display < gems )
        {
            gems_display = gems;
        }
    }

    printf( "Current Gems: %d\n", gems_display );
};

void inventory_add_gems( int amount )
{
    gems += amount;
};

int inventory_get_gems()
{
    return gems;
};
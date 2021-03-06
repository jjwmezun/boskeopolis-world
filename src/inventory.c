#include "inventory.h"
#include "game_state.h"
#include <math.h>

#define GEMS_DISPLAY_INCREMENT 25

static int gems = 0;
static int gems_display = 0;
static treasure_t map_treasures[ 25 ] =
{
    TREASURE_NONE,
    TREASURE_NONE,
    TREASURE_NONE,
    TREASURE_NONE,
    TREASURE_NONE,
    TREASURE_NONE,
    TREASURE_NONE,
    TREASURE_NONE,
    TREASURE_NONE,
    TREASURE_NONE,
    TREASURE_NONE,
    TREASURE_NONE,
    TREASURE_NONE,
    TREASURE_NONE,
    TREASURE_NONE,
    TREASURE_NONE,
    TREASURE_NONE,
    TREASURE_NONE,
    TREASURE_NONE,
    TREASURE_NONE,
    TREASURE_NONE,
    TREASURE_NONE,
    TREASURE_GLOVES,
    TREASURE_NONE,
    TREASURE_NONE
};
static int treasures_collected[ ( int )( TREASURE_NONE ) ];

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
};

void inventory_add_gems( int amount )
{
    gems += amount;
};

int inventory_get_gems()
{
    return gems;
};

int inventory_has_treasure( treasure_t type )
{
    return treasures_collected[ ( int )( type ) ];
};

void inventory_get_treasure( int x )
{
    const int screen = ( int )( floor ( ( double )( x ) / 8.0 ) );
    const treasure_t treasure_type = map_treasures[ screen ];
    if ( treasure_type != TREASURE_NONE )
    {
        treasures_collected[ ( int )( treasure_type ) ] = 1;
        game_state_add_treasure_message( treasure_type );
    }
};

void inventory_init()
{
    // Start with no treasures.
    for ( int i = 0; i < ( int )( TREASURE_NONE ); ++i )
    {
        treasures_collected[ i ] = 0;
    }
}

int inventory_get_gem_value( int gem_type )
{
    switch ( gem_type )
    {
        case ( 0 ): return 50;
        case ( 1 ): return 100;
        case ( 2 ): return 200;
        case ( 3 ): return 500;
        case ( 4 ): return 1000;
    }
    return 0;
};
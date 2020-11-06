#ifndef INVENTORY_H
#define INVENTORY_H

typedef enum
{
    TREASURE_GLOVES,
    TREASURE_NONE
} treasure_t;

void inventory_update();
void inventory_add_gems( int amount );
int inventory_get_gems();
int inventory_has_treasure( treasure_t type );
void inventory_get_treasure( int x );
void inventory_init();

#endif
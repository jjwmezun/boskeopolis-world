#ifndef MAP_H
#define MAP_H

#include "color.h"

typedef struct
{
    int w;
    int h;
    Color bgcolor;
    int * * collision;
    int num_o_collision_layers;
}
Map;

int map_create( Map * map, int state_number );
void map_destroy( Map * map );

#endif // MAP_H
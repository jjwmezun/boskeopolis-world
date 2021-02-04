#ifndef MAP_H
#define MAP_H

#include <stdint.h>

struct camera_t;

#define NUM_O_LAYER_TYPES 6

typedef struct map_t
{
    int_fast16_t * collision;
    int bg_graphics_ids[ NUM_O_LAYER_TYPES ];
    int width;
    int height;
} map_t;

void map_update( const struct map_t * map, const struct camera_t * camera );
int map_index( const struct map_t * map, int x, int y );
int map_test_pixel_solid_collision( const struct map_t * map, int x, int y );
int map_test_pixel_top_solid_collision( const struct map_t * map, int x, int y );
int map_test_pixel_ladder_collision( const struct map_t * map, int x, int y );
int map_test_pixel_gem_collision( const struct map_t * map, int x, int y );
int map_test_pixel_treasure_collision( const struct map_t * map, int x, int y );
void map_remove( struct map_t * map, int x, int y );
map_t map_create();
void map_destroy( struct map_t * map );

#endif
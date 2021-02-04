#ifndef MAP_H
#define MAP_H

#include <stdint.h>

struct camera_t;
struct game_state_level_data_t;

#define NUM_O_LAYER_TYPES 6
#define MAP_WIDTH_LOCATION 0
#define MAP_HEIGHT_LOCATION 1
#define MAP_BG_LOCATION 2
#define MAP_COLLISION_LOCATION ( MAP_BG_LOCATION + NUM_O_LAYER_TYPES )

void map_update( const int * map, const struct camera_t * camera );
int map_index( const int * map, int x, int y );
int map_test_pixel_solid_collision( const int * map, int x, int y );
int map_test_pixel_top_solid_collision( const int * map, int x, int y );
int map_test_pixel_ladder_collision( const int * map, int x, int y );
int map_test_pixel_gem_collision( const int * map, int x, int y );
int map_test_pixel_treasure_collision( const int * map, int x, int y );
int map_create( struct game_state_level_data_t * state );

#endif
#ifndef MAP_H
#define MAP_H

#include <stdint.h>

struct camera_t;
struct game_state_level_data_t;

typedef enum map_obj_type_t
{
    MAP_OBJ_NULL,
    MAP_OBJ_GEM1,
    MAP_OBJ_GEM2,
    MAP_OBJ_GEM3,
    MAP_OBJ_GEM4,
    MAP_OBJ_GEM5
} map_obj_type_t;

typedef struct map_obj_t
{
    map_obj_type_t type;
    int graphics_id;
    int location;
    int x;
    int y;
} map_obj_t;

#define MAP_OBJ_SIZE 5

#define NUM_O_LAYER_TYPES 6
#define MAP_WIDTH_LOCATION 0
#define MAP_HEIGHT_LOCATION 1
#define MAP_MAX_MONEY_LOCATION 2
#define MAP_BG_LOCATION 3
#define MAP_COLLISION_LOCATION ( MAP_BG_LOCATION + NUM_O_LAYER_TYPES )
#define map_obj_number_location( map ) MAP_COLLISION_LOCATION + ( map[ MAP_WIDTH_LOCATION ] * map[ MAP_HEIGHT_LOCATION ] )
#define map_obj_number( map ) map[ map_obj_number_location( map ) ]
#define map_obj( map, i ) ( map_obj_t * )( &map[ map_obj_number_location( map ) + 1 + ( i * MAP_OBJ_SIZE ) ] )

void map_update( const int * map, const struct camera_t * camera, int time );
int map_index( const int * map, int x, int y );
int map_test_pixel_solid_collision( const int * map, int x, int y );
int map_test_pixel_top_solid_collision( const int * map, int x, int y );
int map_test_pixel_ladder_collision( const int * map, int x, int y );
int map_test_pixel_gem_collision( const int * map, int x, int y );
int map_test_pixel_treasure_collision( const int * map, int x, int y );
int map_create( struct game_state_level_data_t * state );
void map_remove( int * map, int x, int y );

#endif
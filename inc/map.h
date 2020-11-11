#ifndef MAP_H
#define MAP_H

#define MAP_WIDTH 200
#define MAP_HEIGHT 32
#define MAP_WIDTH_PIXELS BLOCKS_TO_PIXELS( MAP_WIDTH )
#define MAP_INDEX( x, y ) ( y ) * MAP_WIDTH + ( x )

struct camera_t;

typedef struct map_t
{
    int tiles[ MAP_HEIGHT * MAP_WIDTH ];
} map_t;

void map_render( const struct map_t * map, const struct camera_t * camera );
int map_test_pixel_solid_collision( const struct map_t * map, int x, int y );
int map_test_pixel_top_solid_collision( const struct map_t * map, int x, int y );
int map_test_pixel_ladder_collision( const struct map_t * map, int x, int y );
int map_test_pixel_gem_collision( const struct map_t * map, int x, int y );
int map_test_pixel_treasure_collision( const struct map_t * map, int x, int y );
void map_remove( struct map_t * map, int x, int y );
map_t map_create();

#endif
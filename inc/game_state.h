#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "camera.h"
#include "sprite.h"
#include "treasure.h"

#define MAX_EXTRA 10000

typedef struct game_state_level_data_t
{
    camera_t camera;
    sprite_t player;
    int map_size;
    int extra[ MAX_EXTRA ];
} game_state_level_data_t;

void game_state_update();

void game_state_init();
void game_state_pop();
void game_state_change_level();
void game_state_destroy_all();
void game_state_add_treasure_message( treasure_t treasure );

#endif
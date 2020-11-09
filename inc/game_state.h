#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "treasure.h"

void game_state_update();
void game_state_render();

void game_state_init();
void game_state_pop();
void game_state_change_level();
void game_state_add_treasure_message( treasure_t treasure );

#endif
#ifndef GAME_STATE_MACHINE
#define GAME_STATE_MACHINE

#include "game_state.h"

#define MAX_STATES 5

void state_init();
void state_update();
void state_change( GameState state );
void state_push( GameState state );
void state_pop();

GameState state_create_title();
GameState state_create_level();
GameState state_create_pause();

#endif // GAME_STATE_MACHINE
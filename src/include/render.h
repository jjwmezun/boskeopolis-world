#ifndef RENDER_H
#define RENDER_H

#include "color.h"
#include "graphic.h"
#include "unit.h"

int render_init();
void render_close();
void render_update();
unsigned int render_get_texture_id( const char * local );
unsigned int render_get_texture_id_noindex( const char * local );
void render_clear_textures();
int render_window_should_close();
void * render_get_window();
void render_set_states_number( int number );
void render_clear_graphics();
void render_clear_state_graphics( int state );
unsigned int render_add_graphic( Graphic gfx, int state, Layer layer );
Graphic * render_get_graphic( unsigned int id );

#endif // RENDER_H
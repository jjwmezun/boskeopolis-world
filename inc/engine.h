#ifndef ENGINE_H
#define ENGINE_H

int engine_init();
void engine_close();
int engine_handle_events();
int engine_get_ticks();
void engine_delay( int delay );

#endif
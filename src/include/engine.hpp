#ifndef ENGINE_H
#define ENGINE_H

int engine_init();
void engine_close();
int engine_get_ticks();
int engine_handle_events();

#endif // ENGINE_H
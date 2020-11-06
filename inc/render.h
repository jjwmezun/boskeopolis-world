#ifndef RENDER_H
#define RENDER_H

struct color_t;
struct rect_t;

int render_init( const char* title, int width, int height );
void render_close();
void render_start( const struct color_t* color );
void render_end();
void render_rect( const struct rect_t* rect, const struct color_t* color );

#endif
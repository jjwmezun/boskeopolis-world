#ifndef RENDER_H
#define RENDER_H

struct character_t;
struct color_t;
struct graphics_t;
struct graphics_data_regular_t;
struct rect_t;
struct text_t;

void render_execute();
int render_init( const char * title, int width, int height );
void render_close();
void render_sprite( const struct graphics_data_regular_t * data );
void render_rect( const struct rect_t * rect, const struct color_t * color );
void render_text( const struct text_t * text );
void render_character( const struct character_t * character );
int render_get_texture_id( const char * filename );
int render_add_graphics( const struct graphics_t * gfx );
void render_remove_graphics( int id );
struct graphics_t * render_get_graphics( int id );
int render_create_custom_texture( const char * name, int width, int height );
void render_set_target_texture( int id );
void render_release_target_texture();
void render_clear();
void render_color_canvas( const struct color_t * color );
void render_clear_graphics();

#endif
#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>

struct camera_t;
struct map_t;

typedef enum
{
    SSTATE_FALLING,
    SSTATE_ON_GROUND,
    SSTATE_JUMPING,
    SSTATE_ON_LADDER
} sprite_state_t;

typedef struct sprite_t
{
    double x;
    double y;
    double w;
    double h;
    double accx;
    double accy;
    double vx;
    double vy;
    sprite_state_t state;
    int jump_lock;
} sprite_t;

void sprite_render( const sprite_t* sprite, const struct camera_t* camera, SDL_Renderer* renderer );
double sprite_bottom( const sprite_t* sprite );
double sprite_right( const sprite_t* sprite );
double sprite_center_x( const sprite_t* sprite );
double sprite_center_y( const sprite_t* sprite );
void sprite_update( sprite_t* sprite, const struct map_t* map );

#endif
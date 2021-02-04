#ifndef SPRITE_H
#define SPRITE_H

#include "rect.h"

struct camera_t;

typedef enum
{
    SSTATE_FALLING,
    SSTATE_ON_GROUND,
    SSTATE_JUMPING,
    SSTATE_ON_LADDER
} sprite_state_t;

typedef struct sprite_t
{
    rect_t position;
    double accx;
    double accy;
    double vx;
    double vy;
    sprite_state_t state;
    int jump_lock;
    int graphics_id;
} sprite_t;

sprite_t sprite_create
(
    double x,
    double y,
    double w,
    double h
);

double sprite_bottom( const sprite_t * sprite );
double sprite_right( const sprite_t * sprite );
double sprite_center_x( const sprite_t * sprite );
double sprite_center_y( const sprite_t * sprite );
void sprite_update( sprite_t * sprite, int * map, const struct camera_t * camera );

#endif
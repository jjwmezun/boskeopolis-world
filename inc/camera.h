#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

struct sprite_t;

typedef struct camera_t
{
    double x;
    double y;
    double w;
    double h;
} camera_t;

SDL_Rect camera_relative( const camera_t* camera, SDL_Rect coords );
double camera_bottom( const camera_t* camera );
double camera_right( const camera_t* camera );
void camera_update( camera_t* camera, const struct sprite_t* sprite );

#endif
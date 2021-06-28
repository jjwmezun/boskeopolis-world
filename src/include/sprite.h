#ifndef SPRITE_H
#define SPRITE_H

#include "direction.h"
#include "rect.h"

typedef struct
{
    unsigned int id;
    int walk_frame;
    int walk_timer;
}
GraphicsComponent;

typedef struct
{
    GraphicsComponent gfx;
    Rect position;
    float accx;
    float accy;
    float vx;
    float vy;
    float top_speed;
    float start_speed;
    Direction dir;
    int jump;
}
Sprite;

#define SPRITE_BOTTOM( sprite ) ( ( sprite )->position.y + ( sprite )->position.h )

int sprite_on_ground( Sprite * sprite, Map * map );

Sprite hero_create( int state_number );
void hero_update( Sprite * hero, void * level_state );

#endif // SPRITE_H
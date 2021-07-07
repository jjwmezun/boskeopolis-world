#ifndef RECT_H
#define RECT_H

typedef struct
{
    float x;
    float y;
    float w;
    float h;
}
Rect;

#define RECT_RIGHT( r ) ( ( r )->x + ( r )->w )
#define RECT_BOTTOM( r ) ( ( r )->y + ( r )->h )
#define RECT_RIGHT_DIR( r ) ( ( r ).x + ( r ).w )
#define RECT_BOTTOM_DIR( r ) ( ( r ).y + ( r ).h )

#endif // RECT_H
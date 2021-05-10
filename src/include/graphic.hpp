#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "color.hpp"
#include "rect.hpp"
#include "text.hpp"

struct RectGraphics
{
    Rect rect;
    Color color;
};

struct Graphic
{
    enum class Type
    {
        RECT,
        TEXT
    };

    static Graphic createRect( Rect rect, Color color );
    static Graphic createFullRect( Color color );
    static Graphic createText( Text text );

    Type type;
    union
    {
        RectGraphics rect;
        Text text;
    } data;
};

#endif // GRAPHIC_H
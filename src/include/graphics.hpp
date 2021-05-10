#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "color.hpp"
#include "rect.hpp"
#include "text.hpp"

struct RectGraphics
{
    Rect rect;
    Color color;
};

struct Graphics
{
    enum class Type
    {
        RECT,
        TEXT
    };

    void render();
    static Graphics createRect( Rect rect, Color color );
    static Graphics createFullRect( Color color );
    static Graphics createText( Text text );

    Type type;
    union
    {
        RectGraphics rect;
        Text text;
    } data;
};

#endif // GRAPHICS_H
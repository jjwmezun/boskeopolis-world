#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "color.hpp"
#include "rect.hpp"
#include "text.hpp"
#include <unordered_map>
#include <variant>

struct RectGraphics
{
    Rect rect;
    Color color;
};

struct SpriteGraphics
{
    unsigned int texture;
    unsigned int palette;
    Rect src;
    Rect dest;
    bool flip_x;
    bool flip_y;
    float rotation_x;
    float rotation_y;
    float rotation_z;
};

struct Graphic
{
    enum class Type
    {
        RECT,
        TEXT,
        SPRITE
    };

    static Graphic createRect( Rect rect, Color color );
    static Graphic createFullRect( Color color );
    static Graphic createText( Text text );
    static Graphic createSprite
    (
        unsigned int texture,
        unsigned int palette,
        Rect dest,
        float src_x,
        float src_y,
        std::unordered_map<std::string, std::variant<bool, float>> args = {}
    );

    Type type;
    union
    {
        RectGraphics rect;
        Text text;
        SpriteGraphics sprite;
    } data;
};

#endif // GRAPHIC_H
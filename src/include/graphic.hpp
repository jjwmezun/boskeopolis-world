#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "rect_graphic.hpp"
#include "sprite_graphic.hpp"

class Color;

class Graphic
{
    public:
        static Graphic createRect( Rect rect, Color color );
        static Graphic createSprite( int texture_id, Rect src, Rect dest );
        void setId( int id );
        int getId() const;
        RectGraphic & getRect();
        void render() const;

    private:
        enum class Type
        {
            RECT,
            SPRITE
        };

        Type type_;
        union {
            RectGraphic rect;
            SpriteGraphic sprite;
        } data_;
        int id_;
};

#endif // GRAPHIC_H
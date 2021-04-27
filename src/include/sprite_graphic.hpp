#ifndef SPRITE_GRAPHIC_H
#define SPRITE_GRAPHIC_H

#include "graphic.hpp"
#include "rect.hpp"

class SpriteGraphic : public Graphic
{
    public:
        SpriteGraphic( unsigned int texture_id, unsigned int palette_id, Rect src, Rect dest, bool flip_x = false, bool flip_y = false );
        void render() const override;

    private:
        unsigned int texture_id_;
        unsigned int palette_id_;
        Rect src_;
        Rect dest_;
        bool flip_x_;
        bool flip_y_;
};

#endif // SPRITE_GRAPHIC_H
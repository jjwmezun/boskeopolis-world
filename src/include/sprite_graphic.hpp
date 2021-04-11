#ifndef SPRITE_GRAPHIC_H
#define SPRITE_GRAPHIC_H

#include "graphic.hpp"
#include "rect.hpp"

class SpriteGraphic : public Graphic
{
    public:
        SpriteGraphic( int texture_id, Rect src, Rect dest );
        void render() const override;

    private:
        unsigned int texture_id_;
        Rect src_;
        Rect dest_;
};

#endif // SPRITE_GRAPHIC_H
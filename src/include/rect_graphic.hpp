#ifndef RECT_GRAPHICS_H
#define RECT_GRAPHICS_H

#include "graphic.hpp"
#include "color.hpp"
#include "rect.hpp"

class RectGraphic : public Graphic
{
    public:
        RectGraphic( Rect rect, Color color );
        void render() const override;

    private:
        Rect rect_;
        Color color_;
};

#endif // RECT_GRAPHICS_H
#include "rect_graphic.hpp"
#include "render.hpp"

RectGraphic::RectGraphic( Rect rect, Color color )
:
    rect_ ( rect ),
    color_ ( color )
{};

void RectGraphic::render() const
{
    Render::rect( rect_, color_ );
};
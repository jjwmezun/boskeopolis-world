#include "render.hpp"
#include "text_graphic.hpp"

TextGraphic::TextGraphic( Text text )
:
    text_ ( text )
{};

void TextGraphic::render() const
{
    text_.render();
};
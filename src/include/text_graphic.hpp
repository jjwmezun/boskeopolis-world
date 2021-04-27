#ifndef TEXT_GRAPHIC_H
#define TEXT_GRAPHIC_H

#include "graphic.hpp"
#include "text.hpp"

class TextGraphic : public Graphic
{
    public:
        TextGraphic( Text text );
        ~TextGraphic();
        void render() const override;

    private:
        Text text_;
};

#endif // TEXT_GRAPHIC_H
#ifndef TEXT_H
#define TEXT_H

#include "character.hpp"
#include "color.hpp"
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>
#include "unit.hpp"

struct Text
{
    enum class Align
    {
        LEFT,
        CENTER,
        RIGHT
    };

    enum class VAlign
    {
        TOP,
        MIDDLE,
        BOTTOM
    };

    Text( const char * text, std::unordered_map<const char *, std::variant<float, Align, VAlign, Color>> args = {} );
    void setCharacters( const char * text );

    Align align_ = Align::LEFT;
    VAlign valign_ = VAlign::TOP;
    float x_ = 0;
    float y_ = 0;
    float w_ = Unit::WINDOW_WIDTH_PIXELS;
    float h_ = Unit::WINDOW_HEIGHT_PIXELS;
    float padding_x_ = 0;
    float padding_y_ = 0;
    const char * text_;
    std::vector<Character> characters_;
    Color color_ = { 0, 0, 0, 255 };
};

#endif // TEXT_H
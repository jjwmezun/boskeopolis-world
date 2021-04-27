#ifndef TEXT_H
#define TEXT_H

#include "character.hpp"
#include "color.hpp"
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>

class Text
{
    public:
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

        Text( const char * text, std::unordered_map<const char *, std::variant<double, Align, VAlign, Color>> args = {} );
        ~Text();
        void render() const;

    private:
        std::vector<Character> characters_;
};

#endif // TEXT_H
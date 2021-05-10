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

        static Text create( const char * text, std::unordered_map<const char *, std::variant<double, Align, VAlign, Color>> args = {} );
        void render() const;

    private:
        static constexpr int MAX_CHARACTERS = 256;

        Character characters_[ MAX_CHARACTERS ];
        int number_of_characters_;
        Color color_;
};

#endif // TEXT_H
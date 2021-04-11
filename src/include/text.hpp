#ifndef TEXT_H
#define TEXT_H

#include <string>
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

        Text( const char * text, std::unordered_map<const char *, int> args = {} );
        void render() const;

    private:
        std::vector<std::string> lines_;
};

#endif // TEXT_H
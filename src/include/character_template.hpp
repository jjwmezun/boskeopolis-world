#ifndef CHARACTER_TEMPLATE_H
#define CHARACTER_TEMPLATE_H

struct CharacterTemplate
{
    enum class Type
    {
        NORMAL,
        WHITESPACE,
        NEWLINE
    };

    Type type;
    float w;
    float h;
    float x;
    float y;
};

#endif // CHARACTER_TEMPLATE_H
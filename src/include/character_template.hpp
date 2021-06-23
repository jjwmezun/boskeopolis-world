#ifndef CHARACTER_TEMPLATE_H
#define CHARACTER_TEMPLATE_H

enum CharacterTemplateType
{
    CHAR_NORMAL,
    CHAR_WHITESPACE,
    CHAR_NEWLINE
};

struct CharacterTemplate
{
    CharacterTemplateType type;
    float w;
    float h;
    float x;
    float y;
};

#endif // CHARACTER_TEMPLATE_H
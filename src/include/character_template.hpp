#ifndef CHARACTER_TEMPLATE_H
#define CHARACTER_TEMPLATE_H

enum CharacterTemplateType
{
    CHAR_NORMAL,
    CHAR_WHITESPACE,
    CHAR_NEWLINE
};

typedef struct
{
    CharacterTemplateType type;
    float w;
    float h;
    float x;
    float y;
}
CharacterTemplate;

#endif // CHARACTER_TEMPLATE_H
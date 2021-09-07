#ifndef CHARACTER_TEMPLATE_H
#define CHARACTER_TEMPLATE_H

typedef enum
{
    CHAR_NORMAL,
    CHAR_WHITESPACE,
    CHAR_NEWLINE
}
CharacterTemplateType;

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
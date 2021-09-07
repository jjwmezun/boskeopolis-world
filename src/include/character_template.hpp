#ifndef CHARACTER_TEMPLATE_H
#define CHARACTER_TEMPLATE_H

enum class CharacterType
{
    NORMAL,
    WHITESPACE,
    NEWLINE
};

typedef struct
{
    CharacterType type;
    float w;
    float h;
    float x;
    float y;
}
CharacterTemplate;

#endif // CHARACTER_TEMPLATE_H
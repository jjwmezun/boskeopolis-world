#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include "character_template.hpp"
#include <unordered_map>

struct LocalizationLanguage
{
    std::unordered_map<std::string, CharacterTemplate> chars;
};

namespace Localization
{
    bool init();
    const LocalizationLanguage & getCurrentLanguage();
};

#endif // LOCALIZATION_H
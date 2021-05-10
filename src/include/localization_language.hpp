#ifndef LOCALIZATION_LANGUAGE_H
#define LOCALIZATION_LANGUAGE_H

#include "character_template.hpp"
#include "localization_language.hpp"
#include <string>
#include <unordered_map>

class LocalizationLanguage
{
    public:
        LocalizationLanguage( const char * code );
        const std::unordered_map<std::string, CharacterTemplate>& getCharacterMap() const;

    private:
        std::unordered_map<std::string, CharacterTemplate> character_map_;
};

#endif // LOCALIZATION_LANGUAGE_H
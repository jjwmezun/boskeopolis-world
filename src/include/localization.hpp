#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include "localization_language.hpp"

namespace Localization
{
    void init();
    const LocalizationLanguage& getLanguage();
};

#endif // LOCALIZATION_H
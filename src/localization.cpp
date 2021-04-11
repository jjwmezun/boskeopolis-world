#include "localization.hpp"
#include <stdexcept>
#include <unordered_map>

namespace Localization
{
    std::unordered_map<const char *, LocalizationLanguage> languages_;

    void init()
    {
        languages_.insert( std::pair<const char *, LocalizationLanguage> { "en", { "en" } } );
    };

    const LocalizationLanguage& getLanguage()
    {
        const auto iterator = languages_.find( "en" );
        if ( iterator == languages_.end() )
        {
            throw std::runtime_error( "Could not find EN localization." );
        }
        else
        {
            return iterator->second;
        }
    };
};
#include "filename.hpp"
#include <SDL2/SDL.h>

namespace Filename
{
    #ifdef _WIN32
        static constexpr const char * PATH_DIVIDER = "\\";
    #else
        static constexpr const char * PATH_DIVIDER = "/";
    #endif

    static std::string master_directory;
    static std::string image_directory;
    static std::string localization_directory;

    void init()
    {
		char * base_path_c = SDL_GetBasePath();
		master_directory = std::string( base_path_c ) + "assets" + std::string( PATH_DIVIDER );
		image_directory = master_directory + "graphics" + std::string( PATH_DIVIDER );
		localization_directory = master_directory + "localization" + std::string( PATH_DIVIDER );
		SDL_free( base_path_c );
    };

    std::string image( std::string local )
    {
        return image_directory + local;
    };

    std::string localization( std::string local )
    {
        return localization_directory + local + ".json";
    };
};
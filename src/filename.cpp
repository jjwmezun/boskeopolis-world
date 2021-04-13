#include "filename.hpp"

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
    static std::string shader_directory;

    void init()
    {
		char * base_path_c = "";
		master_directory = std::string( base_path_c ) + "assets" + std::string( PATH_DIVIDER );
		image_directory = master_directory + "graphics" + std::string( PATH_DIVIDER );
		localization_directory = master_directory + "localization" + std::string( PATH_DIVIDER );
		shader_directory = master_directory + "shaders" + std::string( PATH_DIVIDER );
    };

    std::string image( std::string local )
    {
        return image_directory + local;
    };

    std::string localization( std::string local )
    {
        return localization_directory + local + ".json";
    };

    std::string shader( std::string local )
    {
        return shader_directory + local + ".glsl";
    };
};
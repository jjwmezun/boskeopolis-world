#ifndef FILENAME_H
#define FILENAME_H

#include <string>

namespace Filename
{
    void init();
    std::string image( std::string local );
    std::string localization( std::string local );
    std::string shader( std::string local );
};

#endif // FILENAME_H
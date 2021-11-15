#include <algorithm>
#include <cmath>
#include <ctime>
#include "math.hpp"

namespace Math
{
    void initRand()
    {
        srand( time( nullptr ) );
    };

    int randInt( int max_value, int min_value )
    {
        // STD Min & Max makes this safe for value mixups.
        return rand() % ( std::max( max_value, min_value ) + 1 - std::min( min_value, max_value ) ) + std::min( min_value, max_value );
    };
};
#ifndef MATH_H
#define MATH_H

#include <random>

namespace BSW
{
    namespace Math
    {
        void init();
        float randInt( int max, int min = 0.0f );
        std::default_random_engine getRNG();

        template <typename T>
        void shuffle( T & list )
        {
            std::shuffle( std::begin( list ), std::end( list ), getRNG() );
        };
    }
}

#endif // MATH_H
#include "game_state_machine.hpp"
#include "render.hpp"
#include "unit.hpp"

#include <cstdio>

namespace GameStateMachine
{
    GameState states[ Unit::MAX_STATES ];
    unsigned int number_of_states = 0;
    GameState change = {};

    static void initState( GameState & state, unsigned int number );

    struct GSUpdate
    {
        void operator()( std::monostate & state )
        {
        }
        void operator()( auto & state )
        {
            state.update();
        }
    };

    struct GSInit
    {
        void operator()( std::monostate & state )
        {
        }
        void operator()( auto & state )
        {
            state.init( number_of_states );
        }
    };

    void init()
    {
        states[ 0 ] = TitleState();
        initState( states[ 0 ], 0 );
        ++number_of_states;
    };

    void update()
    {
        for ( unsigned int i = 0; i < number_of_states; ++i )
        {
            std::visit( GSUpdate{}, states[ i ] );
        }

        if ( change.index() != 0 )
        {
            number_of_states = 0;
            Render::clearGraphics();
            states[ 0 ] = change;
            initState( states[ 0 ], number_of_states - 1 );
            number_of_states = 1;
            change = {};
        }
    };

    void changeState( GameState state )
    {
        change = state;
    };

    static void initState( GameState & state, unsigned int number )
    {
        std::visit( GSInit{}, state );
    };
};
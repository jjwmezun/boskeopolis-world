#include "game_state_machine.hpp"
#include "render.hpp"
#include "unit.hpp"

#include <cstdio>

namespace GameStateMachine
{
    GameState states[ Unit::MAX_STATES ];
    unsigned int number_of_states = 0;
    GameState change = {};
    std::vector<GameState> push = {};
    int pop = 0;

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
        /*
        for ( unsigned int i = 0; i < number_of_states; ++i )
        {
            std::visit( GSUpdate{}, states[ i ] );
        }
        */

        std::visit( GSUpdate{}, states[ number_of_states - 1 ] );

        if ( change.index() != 0 )
        {
            number_of_states = 0;
            Render::clearGraphics();
            states[ 0 ] = change;
            initState( states[ 0 ], number_of_states );
            number_of_states = 1;
            change = {};
        }

        if ( push.size() > 0 )
        {
            for ( long unsigned int i = 0; i < push.size(); ++i )
            {
                states[ number_of_states ] = push[ i ];
                initState( states[ number_of_states ], number_of_states );
                ++number_of_states;
            }
            push.clear();
        }

        while ( pop > 0 )
        {
            Render::clearStateGraphics( --number_of_states );
            --pop;
        }
    };

    void changeState( GameState state )
    {
        change = state;
    };

    void pushState( GameState state )
    {
        push.push_back( state );
    };

    void popState()
    {
        ++pop;
    };

    static void initState( GameState & state, unsigned int number )
    {
        std::visit( GSInit{}, state );
    };
};
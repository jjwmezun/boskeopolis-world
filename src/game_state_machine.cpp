#include <cassert>
#include "game_state.hpp"
#include "game_state_machine.hpp"
#include "render.hpp"
#include <stdexcept>
#include <vector>

namespace GameStateMachine
{
    static constexpr int MAX_STATES = 5;
    static GameState states[ MAX_STATES ];
    static int number_of_states = 0;

    void init()
    {
        pushState( GameState::createTitle() );
    };

    void update()
    {
        assert( number_of_states > 0 );
        int i = 0;
        while ( i < number_of_states - 1 )
        {
            states[ i ].update( false );
            ++i;
        }
        states[ i ].update( true );
    };

    void render()
    {
        for ( int i = 0; i < number_of_states; ++i )
        {
            states[ i ].render();
        }
    };

    void pushState( GameState state )
    {
        if ( number_of_states == MAX_STATES - 1 )
        {
            throw std::runtime_error( "Too many game states pushed onto stack." );
        }
        states[ number_of_states ] = state;
        states[ number_of_states ].init();
        ++number_of_states;
    };

    void changeState( GameState state )
    {
        for ( int i = 0; i < number_of_states; ++i )
        {
            states[ i ].close();
        }
        Render::clearTextures();
        states[ 0 ] = state;
        states[ 0 ].init();
        number_of_states = 1;
    };

    void popState()
    {
        assert( number_of_states > 0 );
        states[ --number_of_states ].close();
    };
};
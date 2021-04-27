#include <cassert>
#include "game_state.hpp"
#include "game_state_machine.hpp"
#include <memory>
#include "render.hpp"
#include <stdexcept>
#include "title_state.hpp"
#include <vector>

namespace GameStateMachine
{
    std::vector<std::unique_ptr<GameState>> states;
    GameState * change_state = nullptr;
    std::vector<GameState *> push_states;
    int pops = 0;

    void init()
    {
        states.emplace_back( std::unique_ptr<GameState> ( new TitleState() ) );
        states[ states.size() - 1 ]->init();
    };

    void update()
    {
        assert( states.size() > 0 );
        int i = 0;
        while ( i < ( int )( states.size() ) - 1 )
        {
            states[ i ]->update( false );
            ++i;
        }
        states[ i ]->update( true );

        if ( change_state != nullptr )
        {
            for ( auto & state : states )
            {
                state->close();
            }
            states.clear();
            Render::clearTextures();
            states.emplace_back( std::unique_ptr<GameState> ( change_state ) );
            states[ states.size() - 1 ]->init();
            change_state = nullptr;
        }
        else
        {
            while ( pops > 0 )
            {
                states[ states.size() - 1 ]->close();
                states.pop_back();
                --pops;
            }

            for ( auto * state : push_states )
            {
                states.emplace_back( state );
                states[ states.size() - 1 ]->init();
                push_states.clear();
            }
        }
    };

    void render()
    {
        for ( auto & state : states )
        {
            state->render();
        }
    };

    void pushState( GameState * state )
    {
        push_states.emplace_back( state );
    };

    void changeState( GameState * state )
    {
        change_state = state;
    };

    void popState()
    {
        ++pops;
    };
};
#include <cassert>
#include "color.hpp"
#include "game_state_machine.hpp"
#include "input.hpp"
#include <memory>
#include "render.hpp"
#include <stdexcept>
#include <vector>

#include "text.hpp"

namespace GameStateMachine
{
    static constexpr int MAX_STATES = 5;
    GameState states[ MAX_STATES ];
    int number_of_states = 0;

    static void clearStates();
    static void closeState( GameState & state );
    static void initState( GameState & state );

    void init()
    {
        printf( "%ld\n", sizeof( GameState ) );
        changeStateTitle();
    };

    void update()
    {
        switch ( states[ number_of_states - 1 ].type )
        {
            case ( GameState::Type::TITLE ):
            {
                if ( Input::pressedRight() )
                {
                    changeStateLevel();
                }
            }
            break;
            case ( GameState::Type::LEVEL ):
            {
                if ( Input::pressedLeft() )
                {
                    changeStateTitle();
                }
            }
            break;
        }
    };

    void render()
    {
        for ( int i = 0; i < number_of_states; ++i )
        {
            states[ i ].graphics.render();
        }
    };

    void changeStateTitle()
    {
        clearStates();
        states[ 0 ].type = GameState::Type::TITLE;
        number_of_states = 1;
        initState( states[ 0 ] );
    };

    void changeStateLevel()
    {
        clearStates();
        states[ 0 ].type = GameState::Type::LEVEL;
        number_of_states = 1;
        initState( states[ 0 ] );
    };

    void popState()
    {
    };

    static void clearStates()
    {
        while ( number_of_states > 0 )
        {
            closeState( states[ --number_of_states ] );
        }
    };

    static void closeState( GameState & state )
    {
    };

    static void initState( GameState & state )
    {
        state.graphics.init();
        switch ( state.type )
        {
            case ( GameState::Type::TITLE ):
            {
                state.graphics.addGraphic( Graphics::createFullRect( { 255.0, 255.0, 255.0, 255.0 } ), Unit::Layer::BG_1 );
                state.graphics.addGraphic( Graphics::createText( Text::create( "Boskeopolis World", {{ "x", 16.0 }, { "y", 16.0 } } ) ), Unit::Layer::FG_1 );
            }
            break;
            case ( GameState::Type::LEVEL ):
            {

            }
            break;
        }
    };
};
#include <cassert>
#include "color.hpp"
#include "game_state_machine.hpp"
#include "graphic.hpp"
#include "input.hpp"
#include "render.hpp"
#include "text.hpp"

namespace GameStateMachine
{
    GameState states[ MAX_STATES ];
    int number_of_states = 0;

    static void closeState( int number );
    static void initState( int number );

    void init()
    {
        changeState( createTitleState() );
    };

    void update()
    {
        switch ( states[ number_of_states - 1 ].type )
        {
            case ( GameState::Type::TITLE ):
            {
                if ( Input::pressedRight() )
                {
                    changeState( createLevelState() );
                }
            }
            break;
            case ( GameState::Type::LEVEL ):
            {
                if ( Input::pressedRight() )
                {
                    pushState( createPauseState() );
                }
                else if ( Input::pressedLeft() )
                {
                    changeState( createTitleState() );
                }
            }
            break;
            case ( GameState::Type::PAUSE ):
            {
                if ( Input::pressedRight() )
                {
                    changeState( createTitleState() );
                }
                else if ( Input::pressedLeft() )
                {
                    popState();
                }
            }
            break;
        }
    };

    void changeState( GameState state )
    {
        while ( number_of_states > 0 )
        {
            closeState( --number_of_states );
        }
        number_of_states = 1;
        Render::setNumberOfStates( number_of_states );
        Render::clearGraphics();
        states[ 0 ] = state;
        initState( 0 );
    };

    void pushState( GameState state )
    {
        states[ number_of_states ] = state;
        Render::setNumberOfStates( number_of_states );
        initState( number_of_states );
        ++number_of_states;
    };

    void popState()
    {
        closeState( --number_of_states );
        Render::clearStateGraphics( number_of_states );
        Render::setNumberOfStates( number_of_states );
    };

    GameState createTitleState()
    {
        return { GameState::Type::TITLE };
    };

    GameState createLevelState()
    {
        return { GameState::Type::LEVEL };
    };

    GameState createPauseState()
    {
        return { GameState::Type::PAUSE };
    };

    static void closeState( int number )
    {
    };

    static void initState( int number )
    {
        switch ( states[ number ].type )
        {
            case ( GameState::Type::TITLE ):
            {
                Render::addGraphic( Graphic::createFullRect( { 255.0, 255.0, 255.0, 255.0 } ), number, Unit::Layer::BG_1 );
                Render::addGraphic( Graphic::createText( Text::create( "Boskeopolis World", {{ "x", 16.0 }, { "y", 16.0 } } ) ), number, Unit::Layer::FG_1 );
            }
            break;
            case ( GameState::Type::LEVEL ):
            {
                Render::addGraphic( Graphic::createFullRect( { 0.0, 0.0, 255.0, 255.0 } ), number, Unit::Layer::BG_1 );
            }
            break;
            case ( GameState::Type::PAUSE ):
            {
                Render::addGraphic( Graphic::createRect( { 32.0, 32.0, 320.0, 128.0 }, { 255.0, 0.0, 0.0, 255.0 } ), number, Unit::Layer::BG_1 );
            }
            break;
        }
    };
};
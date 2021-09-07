#include "game_state_machine.hpp"
#include "graphic.hpp"
#include "input.hpp"
#include "render.hpp"
#include "text.hpp"
#include "unit.hpp"

#include <cstdio>

struct GSUpdate
{
    void operator()( std::monostate & state )
    {
    }
    void operator()( TitleState & title )
    {
        if ( Input::pressedConfirm() )
        {
            GameStateMachine::changeState( GameStateMachine::createLevelState() );
        }

    }
    void operator()( LevelState & level )
    {
        if ( Input::pressedConfirm() )
        {
            GameStateMachine::changeState( GameStateMachine::createTitleState() );
        }
    }
};

struct GSInit
{
    void operator()( std::monostate & state, unsigned int number )
    {
    }
    void operator()( TitleState & title, unsigned int number )
    {
        Render::addGraphic( Graphic::createFullRect( { 255.0f, 255.0f, 255.0f, 255.0f } ), number, Layer::BG_1 );
    }
    void operator()( LevelState & level, unsigned int number )
    {
        Render::addGraphic( Graphic::createFullRect( { 0.0f, 255.0f, 255.0f, 255.0f } ), number, Layer::BG_1 );
    }
};

namespace GameStateMachine
{
    GameState states[ Unit::MAX_STATES ];
    unsigned int number_of_states = 0;
    GameState change = {};

    static void initState( GameState & state, unsigned int number );

    void init()
    {
        states[ 0 ] = TitleState( 2.5f );
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
            number_of_states = 1;
            Render::clearGraphics();
            states[ 0 ] = change;
            initState( states[ 0 ], number_of_states - 1 );
            change = {};
        }
    };

    GameState createTitleState()
    {
        return TitleState( 0.0f );
    };

    GameState createLevelState()
    {
        return LevelState( "HELLO" );
    };

    void changeState( GameState state )
    {
        change = state;
    };

    static void initState( GameState & state, unsigned int number )
    {
        switch ( state.index() )
        {
            case 0:
            {
            }
            break;
            case 1:
            {
                Render::addGraphic( Graphic::createFullRect( { 255.0f, 255.0f, 255.0f, 255.0f } ), number, Layer::BG_1 );
                Render::addGraphic( Graphic::createText( { "Boskeopolis World", { { "align", Text::Align::CENTER }, { "x_padding", 8.0f }, { "y_padding", 8.0f } } } ), number, Layer::BLOCKS_1 );
            }
            break;
            case 2:
            {
                Render::addGraphic( Graphic::createFullRect( { 0.0f, 255.0f, 255.0f, 255.0f } ), number, Layer::BG_1 );
            }
            break;
        }
    };
};
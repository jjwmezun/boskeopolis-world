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
                auto & hero = states[ number_of_states - 1 ].hero;
                if ( Input::heldRight() )
                {
                    hero.accx = 0.25;
                    hero.dir = Direction::RIGHT;
                }
                else if ( Input::heldLeft() )
                {
                    hero.accx = -0.25;
                    hero.dir = Direction::LEFT;
                }
                else
                {
                    hero.accx = 0.0;
                    hero.vx /= 1.15;
                }

                hero.vx = std::max( -hero.top_speed, std::min( hero.top_speed, hero.vx + hero.accx ) );
                hero.position.x += hero.vx;

                Graphic & g = Render::getGraphic( hero.gfx );
                g.data.sprite.dest = hero.position;

                if ( hero.dir == Direction::RIGHT )
                {
                    if ( g.data.sprite.rotation_y < 180.0 )
                    {
                        g.data.sprite.rotation_y += 5.0;
                    }
                }
                else
                {
                    if ( g.data.sprite.rotation_y > 0.0 )
                    {
                        g.data.sprite.rotation_y -= 5.0;
                    }
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
                states[ number ].hero = { 0, { 32.0, 32.0, 16.0, 25.0 }, 0.0, 0.0, 0.0, 0.0, 4.0, Direction::LEFT };
                states[ number ].hero.gfx = Render::addGraphic
                (
                    Graphic::createSprite
                    (
                        Render::getTextureId( "sprites/autumn.png" ),
                        0,
                        states[ number ].hero.position,
                        0.0,
                        0.0
                    ),
                    number,
                    Unit::Layer::BEFORE_SPRITES_1
                );
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
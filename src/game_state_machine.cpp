#include <cassert>
#include "color.hpp"
#include "game_state_machine.hpp"
#include "graphic.hpp"
#include "input.hpp"
#include "io.hpp"
#include "jsmn/jsmn.h"
#include "render.hpp"
#include "text.hpp"
#include <stdexcept>
#include "vm.hpp"

namespace GameStateMachine
{
    GameState states[ MAX_STATES ];
    int number_of_states = 0;
    VM vm;

    static void closeState( int number );
    static void initState( int number );

    void init()
    {
        changeState( createTitleState() );

        char * sprite_data = io_read( "assets/sprites/demo.json" );

        jsmn_parser parser;
        jsmn_init( &parser );
        size_t num_o_tokens = jsmn_parse( &parser, sprite_data, strlen( sprite_data ), NULL, 0 );
        jsmntok_t tokens[ num_o_tokens ];
        jsmn_parse( &parser, sprite_data, strlen( sprite_data ), tokens, num_o_tokens );

        vm_init( &vm );
        vm_code_push_instruction( &vm.code, OP_RETURN, 123 );
        vm_code_interpret( &vm );
        vm_close( &vm );
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
                auto & hero = states[ number_of_states - 1 ].data.level.hero;
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
        switch ( states[ number ].type )
        {
            case ( GameState::Type::TITLE ):
            {
            }
            break;
            case ( GameState::Type::LEVEL ):
            {
            }
            break;
            case ( GameState::Type::PAUSE ):
            {
            }
            break;
        }
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
                states[ number ].data.level.hero.position.x = 32.0;
                states[ number ].data.level.hero.position.y = 32.0;
                states[ number ].data.level.hero.position.w = 16.0;
                states[ number ].data.level.hero.position.h = 25.0;
                states[ number ].data.level.hero.top_speed = 4.0;
                states[ number ].data.level.hero.gfx = Render::addGraphic
                (
                    Graphic::createSprite
                    (
                        Render::getTextureId( "sprites/autumn.png" ),
                        0,
                        states[ number ].data.level.hero.position,
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
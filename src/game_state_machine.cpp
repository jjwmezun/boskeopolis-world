#include "bytecode_machine.hpp"
#include <cassert>
#include "color.hpp"
#include "game_state_machine.hpp"
#include "graphic.hpp"
#include "input.hpp"
#include "render.hpp"
#include "text.hpp"
#include <stdexcept>

namespace GameStateMachine
{
    GameState states[ MAX_STATES ];
    int number_of_states = 0;
    Bytecode bytecode;

    static void closeState( int number );
    static void initState( int number );

    void init()
    {
        bytecode = bytecode_create
        (
            {
                vm_create_float( 0.25 ),
                vm_create_float( 1.15 ),
                vm_create_float( 180.0 ),
                vm_create_float( 5.0 ),
                vm_create_float( 0.0 )
            },
            {
                // If right not held, skip to TEST_MOVE_LEFT
                VM_LITERAL, 14,
                VM_GET_RIGHT_HELD,
                VM_CNJMP,

                // accx = 0.25
                VM_CONST, 0x00,
                VM_SET_PLAYER_ACCX,

                // dir = 01
                VM_LITERAL, 0x01,
                VM_SET_PLAYER_PROP, 0x00,

                // Skip to SET_VX ( treat next condition as else if )
                VM_LITERAL, 37,
                VM_JMP,

                // TEST_MOVE_LEFT:
                // If left not held, skip to TEST_NO_MOVE
                VM_LITERAL, 29,
                VM_GET_LEFT_HELD,
                VM_CNJMP,

                // accx = -0.25
                VM_CONST, 0x00,
                VM_NEG,
                VM_SET_PLAYER_ACCX,

                // dir = 00
                VM_LITERAL, 0x00,
                VM_SET_PLAYER_PROP, 0x00,

                // Skip to SET_VX
                VM_LITERAL, 37,
                VM_JMP,

                // TEST_NO_MOVE
                VM_CONST, 0x04,
                VM_SET_PLAYER_ACCX,
                VM_CONST, 0x01,
                VM_GET_PLAYER_VX,
                VM_DIV,
                VM_SET_PLAYER_VX,

                // SET_VX:
                // hero.vx = std::max( -hero.top_speed, std::min( hero.top_speed, hero.vx + hero.accx ) )
                VM_GET_PLAYER_VX,
                VM_GET_PLAYER_ACCX,
                VM_ADD,
                VM_GET_PLAYER_TOP_SPEED,
                VM_MIN,
                VM_GET_PLAYER_TOP_SPEED,
                VM_NEG,
                VM_MAX,
                VM_SET_PLAYER_VX,

                // x += vx
                VM_GET_PLAYER_X,
                VM_GET_PLAYER_VX,
                VM_ADD,
                VM_SET_PLAYER_X,

                // 50. if ( hero.dir == Direction::RIGHT )
                VM_LITERAL, 73,
                VM_LITERAL, 0x01,
                VM_GET_PLAYER_PROP, 0x00,
                VM_EQUAL,
                VM_CNJMP,

                // if ( g.data.sprite.rotation_y < 180.0 )
                VM_LITERAL, 85,
                VM_CONST, 0x02,
                VM_GET_PLAYER_GRAPHICS_ROTATION_Y,
                VM_LESS_THAN,
                VM_CNJMP,

                // g.data.sprite.rotation_y += 5.0;
                VM_CONST, 0x03,
                VM_GET_PLAYER_GRAPHICS_ROTATION_Y,
                VM_ADD,
                VM_SET_PLAYER_GRAPHICS_ROTATION_Y,

                // Skip to UPDATE_GFX
                VM_LITERAL, 85,
                VM_JMP,

                // 74. else if ( g.data.sprite.rotation_y > 0.0 )
                VM_LITERAL, 85,
                VM_CONST, 0x04,
                VM_GET_PLAYER_GRAPHICS_ROTATION_Y,
                VM_GREATER_THAN,
                VM_CNJMP,

                // g.data.sprite.rotation_y -= 5.0;
                VM_CONST, 0x03,
                VM_GET_PLAYER_GRAPHICS_ROTATION_Y,
                VM_SUB,
                VM_SET_PLAYER_GRAPHICS_ROTATION_Y,

                // UPDATE_GFX:
                VM_UPDATE_PLAYER_GRAPHICS_POSITION
            }
        );
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
                auto & hero = states[ number_of_states - 1 ].data.level.hero;
                BytecodeMachine::run( bytecode, states[ number_of_states - 1 ].data.level );
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
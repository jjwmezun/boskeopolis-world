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
    enum Instruction
    {
        LITERAL,
        BRK,
        JMP,
        CJMP,
        CNJMP,
        EQUAL,
        NOTEQUAL,
        LESS_THAN,
        GREATER_THAN,
        LESS_THAN_EQUAL,
        GREATER_THAN_EQUAL,
        ADD,
        SUB,
        MUL,
        DIV,
        MAX,
        MIN,
        NEG,
        PRINT_STACK,
        PRINT_STATIC,
        GET_RIGHT_HELD,
        GET_LEFT_HELD,
        GET_PLAYER_X,
        SET_PLAYER_X,
        GET_PLAYER_ACCX,
        SET_PLAYER_ACCX,
        GET_PLAYER_VX,
        SET_PLAYER_VX,
        GET_PLAYER_PROP,
        SET_PLAYER_PROP,
        GET_PLAYER_TOP_SPEED,
        SET_PLAYER_TOP_SPEED,
        UPDATE_PLAYER_GRAPHICS_POSITION,
        GET_PLAYER_GRAPHICS_ROTATION_Y,
        SET_PLAYER_GRAPHICS_ROTATION_Y
    };
    GameState states[ MAX_STATES ];
    int number_of_states = 0;
    std::vector<int_fast16_t> bytecode
    {
        4,
        ( int_fast16_t )( 'd' ),
        ( int_fast16_t )( 'i' ),
        ( int_fast16_t )( 'r' ),
        ( int_fast16_t )( '\0' ),

        // If right not held, skip to TEST_MOVE_LEFT
        LITERAL, 20,
        GET_RIGHT_HELD,
        CNJMP,

        // accx += 0.25
        LITERAL, 94193976344576,
        SET_PLAYER_ACCX,

        // dir = 01
        LITERAL, 0x01,
        LITERAL, 0x01,
        SET_PLAYER_PROP,

        // Skip to SET_VX ( treat next condition as else if )
        LITERAL, 44,
        JMP,

        // TEST_MOVE_LEFT:
        // If left not held, skip to TEST_NO_MOVE
        LITERAL, 36,
        GET_LEFT_HELD,
        CNJMP,

        // accx -= 0.25
        LITERAL, 94193976344576,
        NEG,
        SET_PLAYER_ACCX,

        // dir = 00
        LITERAL, 0x00,
        LITERAL, 0x01,
        SET_PLAYER_PROP,

        // Skip to SET_VX
        LITERAL, 44,
        JMP,

        // TEST_NO_MOVE
        LITERAL, 0x00,
        SET_PLAYER_ACCX,
        LITERAL, 94503232025395,
        GET_PLAYER_VX,
        DIV,
        SET_PLAYER_VX,

        // SET_VX:
        // hero.vx = std::max( -hero.top_speed, std::min( hero.top_speed, hero.vx + hero.accx ) )
        GET_PLAYER_VX,
        GET_PLAYER_ACCX,
        ADD,
        GET_PLAYER_TOP_SPEED,
        MIN,
        GET_PLAYER_TOP_SPEED,
        NEG,
        MAX,
        SET_PLAYER_VX,

        // x += vx
        GET_PLAYER_X,
        GET_PLAYER_VX,
        ADD,
        SET_PLAYER_X,

        // 57. if ( hero.dir == Direction::RIGHT )
        LITERAL, 81,
        LITERAL, 0x01,
        LITERAL, 0x01,
        GET_PLAYER_PROP,
        EQUAL,
        CNJMP,

        // if ( g.data.sprite.rotation_y < 180.0 )
        LITERAL, 93,
        LITERAL, 94297134465024,
        GET_PLAYER_GRAPHICS_ROTATION_Y,
        LESS_THAN,
        CNJMP,

        // g.data.sprite.rotation_y += 5.0;
        LITERAL, 94340040884224,
        GET_PLAYER_GRAPHICS_ROTATION_Y,
        ADD,
        SET_PLAYER_GRAPHICS_ROTATION_Y,

        // Skip to UPDATE_GFX
        LITERAL, 93,
        JMP,

        // else if ( g.data.sprite.rotation_y > 0.0 )
        LITERAL, 93,
        LITERAL, 0x00,
        GET_PLAYER_GRAPHICS_ROTATION_Y,
        GREATER_THAN,
        CNJMP,

        // g.data.sprite.rotation_y -= 5.0;
        LITERAL, 94340040884224,
        GET_PLAYER_GRAPHICS_ROTATION_Y,
        SUB,
        SET_PLAYER_GRAPHICS_ROTATION_Y,

        // UPDATE_GFX:
        UPDATE_PLAYER_GRAPHICS_POSITION
    };

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
                delete states[ number ].data.level.hero.props;
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

                states[ number ].data.level.hero = { 0, { 32.0, 32.0, 16.0, 25.0 }, 0.0, 0.0, 0.0, 0.0, 4.0, nullptr };
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

                states[ number ].data.level.hero.props = new std::unordered_map<std::string, int_fast16_t>();
                states[ number ].data.level.hero.props->insert( { "dir", ( int_fast16_t )( Direction::LEFT ) } );
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
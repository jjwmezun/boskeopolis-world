#include <assert.h>
#include "assoc_array.hpp"
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
                if ( input_pressed_right() )
                {
                    changeState( createLevelState() );
                }
            }
            break;
            case ( GameState::Type::LEVEL ):
            {
                auto & hero = states[ number_of_states - 1 ].data.level.hero;
                Graphic & gfx = render_get_graphic( hero.gfx );
                ++gfx.data.sprite.dest.x;
                if ( input_pressed_left() )
                {
                    pushState( createPauseState() );
                }
            }
            break;
            case ( GameState::Type::PAUSE ):
            {
                if ( input_pressed_right() )
                {
                    changeState( createTitleState() );
                }
                else if ( input_pressed_left() )
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
        render_set_states_number( number_of_states );
        render_clear_graphics();
        states[ 0 ] = state;
        initState( 0 );
    };

    void pushState( GameState state )
    {
        states[ number_of_states ] = state;
        render_set_states_number( number_of_states );
        initState( number_of_states );
        ++number_of_states;
    };

    void popState()
    {
        closeState( --number_of_states );
        render_clear_state_graphics( number_of_states );
        render_set_states_number( number_of_states );
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
                render_add_graphic( Graphic::createFullRect( { 255.0, 255.0, 255.0, 255.0 } ), number, Unit::Layer::BG_1 );
                AssocArray args = assoc_array_create( -1 );
                assoc_array_add( &args, "y", value_create_float( 16.0 ) );
                assoc_array_add( &args, "align", value_create_int( ALIGN_CENTER ) );
                render_add_graphic( Graphic::createText( text_create( "Boskeopolis World", &args ) ), number, Unit::Layer::FG_1 );
                assoc_array_destroy( &args );
            }
            break;
            case ( GameState::Type::LEVEL ):
            {
                render_add_graphic( Graphic::createFullRect( { 0.0, 0.0, 255.0, 255.0 } ), number, Unit::Layer::BG_1 );
                states[ number ].data.level.hero.position.x = 32.0;
                states[ number ].data.level.hero.position.y = 32.0;
                states[ number ].data.level.hero.position.w = 16.0;
                states[ number ].data.level.hero.position.h = 25.0;
                states[ number ].data.level.hero.top_speed = 4.0;

                states[ number ].data.level.hero.gfx = render_add_graphic
                (
                    Graphic::createSprite
                    (
                        render_get_texture_id( "sprites/autumn.png" ),
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
                render_add_graphic( Graphic::createRect( { 32.0, 32.0, 320.0, 128.0 }, { 255.0, 0.0, 0.0, 255.0 } ), number, Unit::Layer::BG_1 );
            }
            break;
        }
    };
};
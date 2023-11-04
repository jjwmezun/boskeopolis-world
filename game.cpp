#include <cstring>
#include "game.hpp"
#include "text.hpp"

namespace BSW
{
    void Game::init()
    {
        num_o_states_ = 2;
        renderer_.current_state_ = 0;
        states_[ 0 ].type = GameStateType::AREA_NAME;
        states_[ 0 ].data.areaname.text = renderer_.addText
        ({
            { "text", "Blueberry Burroughs" },
            { "color", 2 },
            { "align", Text::Align::CENTER },
            { "valign", Text::Valign::MIDDLE }
        });
        states_[ 0 ].data.areaname.timer = 0.0f;
        renderer_.current_state_ = 1;
        states_[ 1 ].type = GameStateType::FADE_IN;
        states_[ 1 ].data.fade.graphic = renderer_.addScreen( 1 );
        states_[ 1 ].data.fade.speed = 0.0f;
        states_[ 1 ].data.fade.opacity = 1.0f;
    };

    void Game::close()
    {
        while ( num_o_states_ > 0 )
        {
            closeState( --num_o_states_ );
        }
    };

    void Game::update( float dt )
    {
        switch ( states_[ num_o_states_ - 1 ].type )
        {
            case ( GameStateType::FADE_IN ):
            {
                #define DATA states_[ num_o_states_ - 1 ].data.fade

                DATA.speed += 0.01f * dt;
                if ( DATA.speed > 0.1f )
                {
                    DATA.speed = 0.1f;
                }
                DATA.opacity -= DATA.speed * dt;
                DATA.graphic.setOpacity( DATA.opacity );

                if ( DATA.opacity <= 0.0f )
                {
                    popState();
                }

                #undef DATA
            }
            break;
            case ( GameStateType::FADE_TO_AREA ):
            {
                #define DATA states_[ num_o_states_ - 1 ].data.fade

                DATA.speed += 0.01f * dt;
                if ( DATA.speed > 0.1f )
                {
                    DATA.speed = 0.1f;
                }
                DATA.opacity += DATA.speed * dt;
                DATA.graphic.setOpacity( DATA.opacity );

                if ( DATA.opacity >= 1.0f )
                {
                    NasrClearGraphics();
                    renderer_.current_state_ = 0;
                    memset( &states_[ 0 ], 0, sizeof( GameState ) );
                    states_[ 0 ].type = GameStateType::MAIN;
                    states_[ 0 ].data.main.bg = renderer_.addScreen( 2 );
                    states_[ 0 ].data.main.map.init( *this );
                    states_[ 0 ].data.main.sprites.init( *this, states_[ 0 ].data.main.map, 96.0f, 32.0f );
                    renderer_.current_state_ = 1;
                    states_[ 1 ].type = GameStateType::FADE_IN;
                    states_[ 1 ].data.fade.graphic = renderer_.addScreen( 1 );
                    states_[ 1 ].data.fade.speed = 0.0f;
                    states_[ 1 ].data.fade.opacity = 1.0f;
                    num_o_states_ = 2;
                }

                #undef DATA
            }
            break;
            case ( GameStateType::AREA_NAME ):
            {
                #define DATA states_[ num_o_states_ - 1 ].data.areaname
                DATA.timer += dt;
                if ( DATA.timer >= 60.0f )
                {
                    ++renderer_.current_state_;
                    states_[ num_o_states_ ].type = GameStateType::FADE_TO_AREA;
                    states_[ num_o_states_ ].data.fade.graphic = renderer_.addScreen( 1 );
                    states_[ num_o_states_ ].data.fade.speed = 0.0f;
                    states_[ num_o_states_ ].data.fade.opacity = 0.0f;
                    ++num_o_states_;
                }
                #undef DATA
            }
            break;
            case ( GameStateType::MAIN ):
            {
                #define DATA states_[ num_o_states_ - 1 ].data.main
                DATA.sprites.update( *this, DATA.map, dt );
                #undef DATA
            }
            break;
        }
    };

    void Game::popState()
    {
        closeState( num_o_states_ - 1 );
        NasrGraphicsClearState( --num_o_states_ );
        --renderer_.current_state_;
    };

    void Game::closeState( unsigned int n )
    {
        switch ( states_[ n ].type )
        {
            case ( GameStateType::MAIN ):
            {
                states_[ n ].data.main.map.close();
            }
            break;
        }
    };
}
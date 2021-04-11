#include "game_state_machine.hpp"
#include "input.hpp"
#include "level_state.hpp"
#include "title_state.hpp"
#include "text.hpp"
#include "text_graphic.hpp"

void TitleState::init()
{
    graphics.addGraphic
    (
        new TextGraphic
        ({
            "Boskeopolis World",
            {
                { "align", ( int )( Text::Align::CENTER ) },
                { "y", 16 }
            }
        }),
        Unit::Layer::BG_1
    );
};

void TitleState::update( bool top_state )
{
    if ( top_state )
    {
        if ( Input::pressedRight() )
        {
            GameStateMachine::changeState( new LevelState() );
        }
    }
};
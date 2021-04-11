#include "game_state_machine.hpp"
#include "input.hpp"
#include "level_state.hpp"
#include "rect_graphic.hpp"
#include "title_state.hpp"
#include "text.hpp"
#include "text_graphic.hpp"

void TitleState::init()
{
    graphics.addGraphic
    (
        new RectGraphic
        (
            { 0.0, 0.0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS },
            { 255, 255, 255, 255 }
        ),
        Unit::Layer::BG_1
    );
    graphics.addGraphic
    (
        new TextGraphic
        ({
            "Boskeopolis World",
            {
                { "align", Text::Align::CENTER },
                { "y", 16.0 }
            }
        }),
        Unit::Layer::BG_1
    );
    graphics.addGraphic
    (
        new TextGraphic
        ({
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.",
            {
                { "align", Text::Align::RIGHT },
                { "y", 48.0 },
                { "x_padding", 8.0 },
                { "color", Color{ 255, 0, 0, 255 } }
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
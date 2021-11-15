#include "game_state_machine.hpp"
#include "graphic.hpp"
#include "input.hpp"
#include "level_state.hpp"
#include "render.hpp"
#include "title_state.hpp"

void TitleState::update()
{
    if ( Input::pressedConfirm() )
    {
        GameStateMachine::changeState( LevelState() );
    }
};

void TitleState::init( unsigned int state )
{
    Render::addGraphic( Graphic::createFullRect( { 0.0f, 0.0f, 0.0f, 255.0f }, true ), state, Layer::BG_1 );
    Render::addGraphic( Graphic::createText( { "Boskeopolis World", { { "color", Color{ 255.0f, 255.0f, 255.0f, 255.0f } }, { "align", Text::Align::CENTER }, { "x_padding", 8.0f }, { "y_padding", 8.0f } } }, true ), state, Layer::BLOCKS_1 );
};
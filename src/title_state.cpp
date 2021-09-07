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
    Render::addGraphic( Graphic::createFullRect( { 255.0f, 255.0f, 255.0f, 255.0f } ), state, Layer::BG_1 );
    Render::addGraphic( Graphic::createText( { "Boskeopolis World", { { "align", Text::Align::CENTER }, { "x_padding", 8.0f }, { "y_padding", 8.0f } } } ), state, Layer::BLOCKS_1 );
};
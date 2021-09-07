#include "game_state_machine.hpp"
#include "graphic.hpp"
#include "input.hpp"
#include "level_state.hpp"
#include "render.hpp"
#include "text.hpp"
#include "title_state.hpp"

void LevelState::update()
{
    if ( Input::pressedConfirm() )
    {
        GameStateMachine::changeState( TitleState() );
    }
};

void LevelState::init( unsigned int state )
{
    Render::addGraphic( Graphic::createFullRect( { 0.0f, 255.0f, 255.0f, 255.0f } ), state, Layer::BG_1 );
};
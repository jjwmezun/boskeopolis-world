#include "game_state_machine.hpp"
#include "graphic.hpp"
#include "input.hpp"
#include "render.hpp"
#include "treasure_message_state.hpp"
#include "unit.hpp"

TreasureMessageState::TreasureMessageState( int id ) : id ( id ) {};

void TreasureMessageState::update()
{
    if ( Input::pressedConfirm() )
    {
        GameStateMachine::popState();
    }
};

void TreasureMessageState::init( unsigned int state )
{
    Render::addGraphic( Graphic::createRect
    (
        { 64.0f, 64.0f, Unit::WINDOW_WIDTH_PIXELS - 128, Unit::WINDOW_HEIGHT_PIXELS - 128 },
        { 0.0f, 0.0f, 0.0f, 255.0f },
        true
    ), state, Layer::BG_1 );
};
#include "game_state_machine.hpp"
#include "input.hpp"
#include "level_state.hpp"
#include "render.hpp"
#include "sprite_graphic.hpp"
#include "title_state.hpp"
#include "unit.hpp"

void LevelState::init()
{
    graphics.addGraphic
    (
        new SpriteGraphic
        (
            Render::getTextureId( "sprites/autumn.png" ),
            { 0.0, 0.0, 16.0, 25.0 },
            { 192.0, 100.0, 16.0, 25.0 }
        ),
        Unit::Layer::SPRITES_1
    );
};

void LevelState::update( bool top_state )
{
    if ( top_state )
    {
        if ( Input::pressedLeft() )
        {
            GameStateMachine::changeState( new TitleState() );
        }
    }
};
#include "game_state.hpp"
#include "game_state_machine.hpp"
#include "graphic.hpp"
#include "input.hpp"
#include "render.hpp"
#include "unit.hpp"

GameState GameState::createTitle()
{
    GameState g;
    g.type_ = Type::TITLE;
    return g;
};

GameState GameState::createLevel()
{
    GameState g;
    g.type_ = Type::LEVEL;
    return g;
};

void GameState::update( bool top_state )
{
    switch ( type_ )
    {
        case ( Type::TITLE ):
        {
            if ( top_state )
            {
                if ( Input::heldRight() )
                {
                    GameStateMachine::changeState( createLevel() );
                }
            }
        }
        break;
        case ( Type::LEVEL ):
        {
            if ( Input::heldLeft() )
            {
                    GameStateMachine::changeState( createTitle() );
            }
        }
        break;
    }
};

void GameState::init()
{
    graphics.init();
    switch ( type_ )
    {
        case ( Type::TITLE ):
        {
            graphics.addGraphic( Graphic::createRect( { 0.0, 0.0, 400.0, 224.0 }, { 255, 0, 255, 255 } ), Unit::Layer::BG_1 );
        }
        break;
        case ( Type::LEVEL ):
        {
            graphics.addGraphic
            (
                Graphic::createSprite
                (
                    Render::getTextureId( "sprites/autumn.png" ),
                    { 0.0, 0.0, 16.0, 25.0 },
                    { 192.0, 100.0, 16.0, 25.0 }
                ),
                Unit::Layer::SPRITES_1
            );
        }
        break;
    }
};

void GameState::close()
{

};

void GameState::render() const
{
    graphics.render();
};
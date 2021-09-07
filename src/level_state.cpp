#include "game_state_machine.hpp"
#include "graphic.hpp"
#include "hero.hpp"
#include "level_state.hpp"
#include "render.hpp"
#include "text.hpp"

LevelState::LevelState()
:
    hero_ ( Hero::create() )
{};

void LevelState::update()
{
    hero_.update( hero_, *this );
};

void LevelState::init( unsigned int state )
{
    Render::addGraphic( Graphic::createFullRect( { 0.0f, 255.0f, 255.0f, 255.0f } ), state, Layer::BG_1 );
    hero_.gfx = Render::addGraphic( Graphic::createSprite( Render::getTextureID( "sprites/autumn.png" ), 0, hero_.position, 0.0f, 0.0f ), state, Layer::BG_1 );
};
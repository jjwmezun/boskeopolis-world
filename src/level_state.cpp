#include "game_state_machine.hpp"
#include "graphic.hpp"
#include "hero.hpp"
#include "level_state.hpp"
#include "render.hpp"
#include "text.hpp"

LevelState::LevelState()
:
    map ( "blueberry-1" ),
    hero ( Hero::create() )
{};

void LevelState::update()
{
    hero.update( hero, *this );
    Render::adjustCamera( &hero.position, ( float )( Unit::blocksToPixels( map.width ) ), ( float )( Unit::blocksToPixels( map.height ) ) );
    inventory.update();
    rain.update();
};

void LevelState::init( unsigned int state )
{
    Render::resetCamera();
    Render::addGraphic( Graphic::createFullRect( map.bg_color, true ), state, Layer::BG_1 );
    tilesets.init();
    map.init( state, tilesets );
    hero.gfx = Render::addGraphic( Graphic::createSprite( Render::getTextureID( "sprites/autumn.png" ), 128, hero.position, 0.0f, 0.0f ), state, Layer::SPRITES_1 );
    rain.init( state );
    Render::addGraphic( Graphic::createFullRectGradient
    (
        { 200.0f, 224.0f, 248.0f, 80.0f },
        { 200.0f, 224.0f, 248.0f, 80.0f },
        { 48.0f, 80.0f, 96.0f, 160.0f },
        { 48.0f, 80.0f, 96.0f, 160.0f },
        true
    ), state, Layer::FG_2 );
};
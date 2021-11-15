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
    printf( "Gems: %d\n", inventory.getShownGems() );
};

void LevelState::init( unsigned int state )
{
    Render::resetCamera();
    Render::addGraphic( Graphic::createFullRect( map.bg_color, true ), state, Layer::BG_1 );
    tilesets.init();
    map.init( state, tilesets );
    hero.gfx = Render::addGraphic( Graphic::createSprite( Render::getTextureID( "sprites/autumn.png" ), 0, hero.position, 0.0f, 0.0f ), state, Layer::SPRITES_1 );
};
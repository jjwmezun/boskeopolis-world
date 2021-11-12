#include "game_state_machine.hpp"
#include "graphic.hpp"
#include "hero.hpp"
#include "level_state.hpp"
#include "render.hpp"
#include "text.hpp"

LevelState::LevelState()
:
    map_ ( "blueberry-1" ),
    hero_ ( Hero::create() )
{};

void LevelState::update()
{
    hero_.update( hero_, *this );
    Render::adjustCamera( &hero_.position, ( float )( Unit::blocksToPixels( map_.width ) ), ( float )( Unit::blocksToPixels( map_.height ) ) );
};

void LevelState::init( unsigned int state )
{
    Render::addGraphic( Graphic::createFullRect( map_.bg_color, true ), state, Layer::BG_1 );
    map_.init( state );
    hero_.gfx = Render::addGraphic( Graphic::createSprite( Render::getTextureID( "sprites/autumn.png" ), 0, hero_.position, 0.0f, 0.0f ), state, Layer::BG_1 );
};
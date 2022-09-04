#include "crab.hpp"
#include "game_state_machine.hpp"
#include "graphic.hpp"
#include "hero.hpp"
#include "input.hpp"
#include "level_state.hpp"
#include "render.hpp"
#include <stdexcept>
#include "text.hpp"
#include "treasure_list_state.hpp"

LevelState::LevelState()
:
    map ( "blueberry-1" )
{};

void LevelState::update()
{
    for ( auto & sprite : sprites )
    {
        sprite.update( sprite, *this );
    }
    for ( int i = 0; i < sprites.size(); ++i )
    {
        for ( int j = 0; j < sprites.size(); ++j )
        {
            if ( i != j )
            {
                Sprite & a = sprites[ i ];
                Sprite & b = sprites[ j ];
                a.interact( a, b, *this );
                b.interact( b, a, *this );
            }
        }
    }
    Render::adjustCamera( &getHero().position, ( float )( Unit::blocksToPixels( map.width ) ), ( float )( Unit::blocksToPixels( map.height ) ) );
    inventory.update();
    rain.update();
    if ( Input::pressedMenu() )
    {
        GameStateMachine::pushState( TreasureListState( inventory ) );
    }
};

void LevelState::init( unsigned int state )
{
    Render::resetCamera();
    Render::addGraphic( Graphic::createFullRect( map.bg_color, true ), state, Layer::BG_1 );
    tilesets.init();
    map.init( state, tilesets );
    sprites.push_back( Hero::create( state ) );
    for ( const auto & sprite : map.sprites )
    {
        switch ( sprite.type )
        {
            case ( 0 ):
            {
                sprites.push_back( Crab::create( state, sprite.x, sprite.y ) );
            }
            break;
            default:
            {
                throw std::runtime_error( "Invalid sprite type! #" + std::to_string( sprite.type ) + "\n" );
            }
        }
    }
    rain.init( state );
    Render::addGraphic( Graphic::createFullRectGradient
    (
        { 200.0f, 224.0f, 248.0f, 80.0f },
        { 200.0f, 224.0f, 248.0f, 80.0f },
        { 48.0f, 80.0f, 96.0f, 160.0f },
        { 48.0f, 80.0f, 96.0f, 160.0f },
        true
    ), state, Layer::FG_2 );
    inventory.init( state );
};

Sprite & LevelState::getHero()
{
    for ( Sprite & sprite : sprites )
    {
        if ( sprite.hasType( SpriteType::HERO ) )
        {
            return sprite;
        }
    }
    throw std::runtime_error( "¡Missing hero sprite!" );
};
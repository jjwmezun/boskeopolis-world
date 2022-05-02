#include "graphic.hpp"
#include "inventory.hpp"
#include "render.hpp"
#include "string.hpp"
#include "text.hpp"

Inventory::Inventory()
:
    gems ( 0 ),
    shown_gems ( 0 ),
    health ( 12 ),
    target_health( 12 )
{
    for ( int i = 0; i < MAX_TREASURES; ++i )
    {
        treasures_collected[ i ] = false;
    }
};

void Inventory::init( unsigned int state )
{
    Render::addGraphic( Graphic::createRect
    (
        { 4, 196, 392, 24 },
        { 0, 0, 0, 128 },
        true
    ), state, Layer::FG_2 );
    pts_gfx = Render::addGraphic( Graphic::createText
    (
        { getShownGemsString().c_str(), { { "color", Color{ 255, 255, 255, 255 } }, { "x", 12 }, { "y", 204 } } },
        true
    ), state, Layer::AFTER_FG_2 );
    hp_gfx = Render::addGraphic( Graphic::createText
    (
        { getHPString().c_str(), { { "color", Color{ 255, 255, 255, 255 } }, { "x", 100 }, { "y", 204 } } },
        true
    ), state, Layer::AFTER_FG_2 );
};

void Inventory::update()
{
    if ( shown_gems < gems - 25 )
    {
        shown_gems += 25;
        updateGemsGraphics();
    }
    else if ( shown_gems < gems )
    {
        shown_gems = gems;
        updateGemsGraphics();
    }

    if ( target_health < health )
    {
        if ( health - target_health < 0.12 )
        {
            health = target_health;
        }
        else
        {
            health -= 0.1;
        }
        updateHPGraphics();
    }
    else if ( target_health > health )
    {
        if ( target_health - health < 0.12 )
        {
            health = target_health;
        }
        else
        {
            health += 0.2;
        }
        updateHPGraphics();
    }
};

void Inventory::addGems( int amount )
{
    gems += amount;
};

int Inventory::getShownGems() const
{
    return shown_gems;
};

void Inventory::getTreasure( int id )
{
    treasures_collected[ id ] = true;
};

bool Inventory::hasTreasure( TreasureType type ) const
{
    return treasures_collected[ ( int )( type ) ];
};

std::string Inventory::getShownGemsString() const
{
    return std::string( "₧" + String::pad( std::to_string( shown_gems ), 8 ) );
};

void Inventory::updateGemsGraphics()
{
    Graphic * gfx = Render::getGraphic( pts_gfx );
    Text & text = std::get<Text>( gfx->data );
    text.setCharacters( getShownGemsString().c_str() );
};

void Inventory::updateHPGraphics()
{
    Graphic * gfx = Render::getGraphic( hp_gfx );
    Text & text = std::get<Text>( gfx->data );
    text.setCharacters( getHPString().c_str() );
};

void Inventory::hurt( float damage )
{
    target_health = health - damage;
};

std::string Inventory::getHPString() const
{
    return std::string( "HP: " + String::pad( std::to_string( ( int )( std::floor( health ) ) ), 2 ) );
};
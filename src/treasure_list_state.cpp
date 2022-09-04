#include "game_state_machine.hpp"
#include "graphic.hpp"
#include "input.hpp"
#include "render.hpp"
#include "treasure_list_state.hpp"

static constexpr float FADE_SPEED = 8.0f;
static constexpr int TABLE_PADDING = 8;
static constexpr int SQUARE_SIZE = 20;
static constexpr Color BORDER_COLOR = { 255.0f, 0.0f, 0.0f, 255.0f };

TreasureListState::TreasureListState( Inventory _inventory ) :
    inventory ( _inventory )
{};

void TreasureListState::update()
{
    if ( leaving ) {
        if ( opacity <= 0.0f ) {
            GameStateMachine::popState();
        }
        else {
            opacity -= FADE_SPEED;
            updateBGOpacity();
        }
    }
    else {
        if ( opacity < 255.0f ) {
            opacity += FADE_SPEED;
            updateBGOpacity();
        }
        if ( Input::pressedConfirm() )
        {
            leaving = true;
        }
    }
};

void TreasureListState::init( unsigned int state )
{
    leaving = false;
    opacity = 0.0f;
    //const char data[ ]
    /*
    Render::addGraphic( Graphic::createText( { "Treasures", { { "color", Color{ 255.0f, 255.0f, 255.0f, 255.0f } }, { "align", Text::Align::CENTER }, { "x_padding", 16.0f }, { "y_padding", 16.0f } } }, true ), state, Layer::BLOCKS_1 );
    bg = Render::addGraphic( Graphic::createFullRect( { 48.0f, 16.0f, 64.0f, opacity }, true ), state, Layer::BG_1 );
    const int colsize = ( int )( std::floor( ( double )( Unit::WINDOW_WIDTH_PIXELS - ( TABLE_PADDING * 2 ) - 3 ) / ( double )( SQUARE_SIZE + 1 ) ) );
    const int start_x = ( int )( std::round( ( double )( Unit::WINDOW_WIDTH_PIXELS - ( colsize * ( SQUARE_SIZE + 1 ) ) ) / 2.0 ) );
    const int start_y = 24 + std::max( 16, TABLE_PADDING ) + 2;
    const int rowsize = ( int )( std::floor( ( double )( Unit::WINDOW_HEIGHT_PIXELS - TABLE_PADDING - 3 - start_y ) / ( double )( SQUARE_SIZE + 1 ) ) );
    int i = 0;
    for ( int y = start_y; y < Unit::WINDOW_HEIGHT_PIXELS - TABLE_PADDING - 1 - SQUARE_SIZE; y += SQUARE_SIZE + 1 ) {
        for ( int x = start_x; x < Unit::WINDOW_WIDTH_PIXELS - TABLE_PADDING - 1 - SQUARE_SIZE; x += SQUARE_SIZE + 1 ) {
            Render::addGraphic( Graphic::createSprite(
                Render::getTextureID( "inventory/treasures.png" ),
                124,
                { ( float )( x + 2 ), ( float )( y + 2 ), 16.0f, 16.0f },
                ( inventory.hasTreasure( ( TreasureType )( i ) ) ) ? 16.0f : 0.0f,
                0.0f,
                { { "abs", true } }
            ), state, Layer::FG_1 );
            ++i;
        }
    }*/
    /*
    Render::addGraphic( Graphic::createRect( { ( float )( start_x - 1 ), ( float )( start_y - 2 ), ( float )( colsize * ( SQUARE_SIZE + 1 ) + 2 ), 2.0f }, BORDER_COLOR, true ), state, Layer::BLOCKS_1 );
    Render::addGraphic( Graphic::createRect( { ( float )( start_x - 1 ), ( float )( start_y - 1 + rowsize * ( SQUARE_SIZE + 1 ) ), ( float )( colsize * ( SQUARE_SIZE + 1 ) + 2 ), 2.0f }, BORDER_COLOR, true ), state, Layer::BLOCKS_1 );
    Render::addGraphic( Graphic::createRect( { ( float )( start_x - 2 ), ( float )( start_y - 1 ), 2.0f, ( float )( rowsize * ( SQUARE_SIZE + 1 ) + 1 ) }, BORDER_COLOR, true ), state, Layer::BLOCKS_1 );
    for ( int y = start_y; y < Unit::WINDOW_HEIGHT_PIXELS - TABLE_PADDING - 1 - ( SQUARE_SIZE * 2 ); y += SQUARE_SIZE + 1 ) {
        Render::addGraphic( Graphic::createRect( { ( float )( start_x ), ( float )( y + SQUARE_SIZE ), ( float )( colsize * ( SQUARE_SIZE + 1 ) ), 1.0f }, BORDER_COLOR, true ), state, Layer::BLOCKS_1 );
    }
    for ( int x = start_x; x < Unit::WINDOW_WIDTH_PIXELS - TABLE_PADDING - 1 - ( SQUARE_SIZE * 2 ); x += SQUARE_SIZE + 1 ) {
        Render::addGraphic( Graphic::createRect( { ( float )( x + SQUARE_SIZE ), ( float )( start_y ), 1.0f, ( float )( rowsize * ( SQUARE_SIZE + 1 ) ) }, BORDER_COLOR, true ), state, Layer::BLOCKS_1 );
    }*/
};

void TreasureListState::updateBGOpacity()
{
    /*
    Graphic * gfx = Render::getGraphic( bg );
    auto & g = std::get<RectGraphics>( gfx->data );
    g.color.a = opacity;*/
};
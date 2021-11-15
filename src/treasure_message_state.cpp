#include "game_state_machine.hpp"
#include "graphic.hpp"
#include "input.hpp"
#include "render.hpp"
#include "text.hpp"
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
        { 66.0f, 66.0f, Unit::WINDOW_WIDTH_PIXELS - 132, Unit::WINDOW_HEIGHT_PIXELS - 132 },
        { 0.0f, 0.0f, 0.0f, 224.0f },
        true
    ), state, Layer::BG_1 );
    Render::addGraphic( Graphic::createSprite(
        Render::getTextureID( "inventory/treasure-message-border-shadow.png" ),
        127,
        { 64.0f, 64.0f, Unit::WINDOW_WIDTH_PIXELS - 127, Unit::WINDOW_HEIGHT_PIXELS - 127 },
        0.0f,
        0.0f,
        {
            { "abs", true },
            { "opacity", 0.5f }
        }
    ), state, Layer::BG_2 );
    Render::addGraphic( Graphic::createSprite(
        Render::getTextureID( "inventory/treasure-message-border.png" ),
        127,
        { 64.0f, 64.0f, Unit::WINDOW_WIDTH_PIXELS - 128, Unit::WINDOW_HEIGHT_PIXELS - 128 },
        0.0f,
        0.0f,
        {
            { "abs", true }
        }
    ), state, Layer::BG_2 );
    Render::addGraphic( Graphic::createText(
        {
            "¡You collected Climber’s Gloves!",
            {
                { "color", Color{ 255.0f, 255.0f, 255.0f, 255.0f } },
                { "align", Text::Align::CENTER },
                { "valign", Text::VAlign::MIDDLE },
                { "h", 32.0f },
                { "y", 112.0f }
            }
        }
    ), state, Layer::FG_1 );
    Render::addGraphic( Graphic::createSprite(
        Render::getTextureID( "inventory/treasures.png" ),
        124,
        { 192.0f, 88.0f, 16.0f, 16.0f },
        0.0f,
        0.0f,
        { { "abs", true } }
    ), state, Layer::FG_1 );
};
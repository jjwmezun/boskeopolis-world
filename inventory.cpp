#include "config.hpp"
#include "game.hpp"
#include "inventory.hpp"
#include "layer.hpp"

namespace BSW
{
    void Inventory::init( const Game & game )
    {
        money_ = 0.0f;

        game.render().addMenuBox
        (
            Text::Align::CENTER,
            Text::Valign::TOP,
            WINDOW_WIDTH_PIXELS - 32.0f,
            32.0f,
            { { "y", WINDOW_HEIGHT_PIXELS - 48.0f } }
        );

        money_gfx_ = game.render().addCounter
        ({
            { "num", money_ },
            { "numpadding", 9 },
            { "palette", 128 },
            { "color", 1 },
            { "x", 24.0f },
            { "y", WINDOW_HEIGHT_PIXELS - 32.0f },
            { "layer", Layer::AFTER_FG_2 },
            { "maxdigits", 9 }
        });
    };

    void Inventory::updateMoney( float n )
    {
        money_ += n;
        money_gfx_.setNumber( money_ );
    };
}
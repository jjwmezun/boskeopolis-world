#include "color.hpp"
#include "engine.hpp"
#include "game_state_machine.hpp"
#include "layer.hpp"
#include "localization.hpp"
#include "log.hpp"
#include "rect.hpp"
#include "render.hpp"

static constexpr int DT = 17;

static int running = 1;
static int ticks = 0;
static int accumulator = 0;

int main()
{
    if ( !Engine::init() )
    {
        Log::sendError( "¡Error! Failed to initialize game!" );
        return -1;
    }
    Localization::init();
    GameStateMachine::init();

    ticks = Engine::getTicks();
    while ( running )
    {
        running = Engine::handleEvents();
        int new_time = Engine::getTicks();
        int frame_time = new_time - ticks;
        ticks = new_time;
        accumulator += frame_time;
        while ( accumulator >= DT )
        {
            accumulator -= DT;
        }

        GameStateMachine::update();
        Render::update();
    }

    Render::close();
    Engine::close();
    return 0;
}
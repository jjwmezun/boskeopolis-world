#include "color.hpp"
#include <cstdio>
#include "engine.hpp"
#include "filename.hpp"
#include "game_state_machine.hpp"
#include "graphic.hpp"
#include "input.hpp"
#include "rect.hpp"
#include "render.hpp"
#include "unit.hpp"
#include <vector>

static bool running = true;
static int ticks = 0;
static int accumulator = 0;

#define DT 17

int main()
{
    if ( !Engine::init() )
    {
        printf( "¡Error! Failed to initialize game!\n" );
        return -1;
    }
    if ( !Render::init( "Boskeopolis World", Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS, { 0, 0, 0, 255 } ) )
    {
        printf( "¡Error! ¡Failed to initialize game renderer!\n" );
        return -1;
    }

    Filename::init();
    Input::init();
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
            GameStateMachine::update();
            Input::update();
            accumulator -= DT;
        }

        Render::startUpdate();
        GameStateMachine::render();
        Render::endUpdate();
    }

    Render::close();
    Engine::close();
    return 0;
}
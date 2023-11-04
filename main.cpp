#include "config.hpp"
#include "game.hpp"
#include "input.hpp"
#include "nasr.h"
#include "nasr_input.h"
#include "nasr_localization.h"
#include <nasringine/glad/glad.h>
#include "nasringine/GLFW/glfw3.h"

static bool running = 1;
static double fixed_timestep = 0.0f;
static double fixed_timestep_update = 1.0f / fixed_timestep;

float total_dt = 0.0f;
unsigned int dtcount = 0;

int main( int argc, char ** argv )
{
    NasrInit
    (
        "Boskeopolis World",
        BSW::WINDOW_WIDTH_PIXELS,
        BSW::WINDOW_HEIGHT_PIXELS,
        BSW::MAX_STATES,
        128,
        128,
        18,
        NASR_SAMPLING_NEAREST,
        NASR_INDEXED_YES,
        1,
        8
    );

    double prev_time = NasrGetTime();
    double accumulated_time = prev_time;

    //BSL::Math::init();
    BSW::Game game;
    //NasrSetLanguage( "assets/localization/es.json", "boskeopolis-land" );
    NasrSetPalette( "assets/palettes/palette.png" );
    BSW::Input::init();

    //game.changeState( std::make_unique<BSL::TitleState>() );
    game.init();

    while ( running )
    {
        if ( NasrHasClosed() )
        {
            running = false;
        }
        else
        {
            NasrHandleEvents();
            if ( fixed_timestep )
            {
                double current_time = NasrGetTime();
                double timechange = current_time - prev_time;
                accumulated_time += timechange;
                while ( accumulated_time >= fixed_timestep_update )
                {
                    accumulated_time -= fixed_timestep_update;
                    //game.update( 60.0f / static_cast<float>( fixed_timestep ) );
                }
                NasrUpdate( 1.0f );
                prev_time = current_time;
            }
            else
            {
                double current_time = NasrGetTime();
                double timechange = current_time - prev_time;
                double fps = 1.0 / timechange;
                float dt = 60.0f / ( float )( fps );

                total_dt += ( 60.0f / dt );
                ++dtcount;

                game.update( dt );
                NasrUpdate( dt );
                prev_time = current_time;
            }
            NasrInputUpdate();
        }
    }
    game.close();
    NasrClose();
}
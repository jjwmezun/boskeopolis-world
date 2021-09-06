#include "color.h"
#include "engine.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "log.h"
#include <math.h>
#include "render.h"
#include "unit.h"

static void engine_handle_input( GLFWwindow * window, int key, int scancode, int action, int mods );

int engine_init()
{
    glfwInit();
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

    if ( !render_init() )
    {
        log_error( "¡Error! ¡Failed to initialize game renderer!\n" );
        return 0;
    }
    glfwSetKeyCallback( ( GLFWwindow * )( render_get_window() ), &engine_handle_input );
    return 1;
};

void engine_close()
{
    glfwTerminate();
};

int engine_get_ticks()
{
    return ( int )( floor( glfwGetTime() * 1000 ) );
};

int engine_handle_events()
{
    int running = !render_window_should_close();
    if ( running )
    {
        GLFWwindow * window = ( GLFWwindow * )( render_get_window() );
        if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
        {
            glfwSetWindowShouldClose( window, 1 );
        }
        glfwPollEvents();
    }
    return running;
};

static void engine_handle_input( GLFWwindow * window, int key, int scancode, int action, int mods )
{
    switch ( action )
    {
        case ( GLFW_PRESS ):
        {
        }
        break;
        case ( GLFW_RELEASE ):
        {
        }
        break;
    }
}
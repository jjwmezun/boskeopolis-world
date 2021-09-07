#include "color.hpp"
#include "engine.hpp"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "input.hpp"
#include "log.hpp"
#include <cmath>
#include "render.hpp"
#include "unit.hpp"

namespace Engine
{
    static void handle_input( GLFWwindow * window, int key, int scancode, int action, int mods );

    int init()
    {
        glfwInit();
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
        glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
        Input::initKeys
        ({
            { Input::Key::CONFIRM, { ( int )( GLFW_KEY_Z ) } },
            { Input::Key::RIGHT, { ( int )( GLFW_KEY_RIGHT ) } },
            { Input::Key::LEFT, { ( int )( GLFW_KEY_LEFT ) } }
        });
        if ( !Render::init() )
        {
            Log::sendError( "¡Error! ¡Failed to initialize game renderer!" );
            return 0;
        }
        glfwSetKeyCallback( ( GLFWwindow * )( Render::getWindow() ), &handle_input );
        return 1;
    };

    void close()
    {
        glfwTerminate();
    };

    int getTicks()
    {
        return ( int )( floor( glfwGetTime() * 1000 ) );
    };

    int handleEvents()
    {
        int running = !Render::shouldWindowClose();
        if ( running )
        {
            GLFWwindow * window = ( GLFWwindow * )( Render::getWindow() );
            if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
            {
                glfwSetWindowShouldClose( window, 1 );
            }
            glfwPollEvents();
        }
        return running;
    };

    static void handle_input( GLFWwindow * window, int key, int scancode, int action, int mods )
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
    };
}
#include "color.hpp"
#include <cstdio>
#include "engine.hpp"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "input.hpp"
#include "render.hpp"
#include "unit.hpp"

namespace Engine
{
    static void handleInput( GLFWwindow * window, int key, int scancode, int action, int mods );

    bool init()
    {
        glfwInit();
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
        glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
        Input::initKeys
        ({
            { Input::Key::RIGHT, { ( int )( GLFW_KEY_RIGHT ) } },
            { Input::Key::LEFT, { ( int )( GLFW_KEY_LEFT ) } }
        });
        if ( !Render::init( "Boskeopolis World", Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS, { 0, 0, 0, 255 } ) )
        {
            printf( "¡Error! ¡Failed to initialize game renderer!\n" );
            return -1;
        }
        glfwSetKeyCallback( ( GLFWwindow * )( Render::getWindow() ), &handleInput );
        return true;
    };

    void close()
    {
        glfwTerminate();
    };

    int getTicks()
    {
        return ( int )( floor( glfwGetTime() * 1000 ) );
    };

    bool handleEvents()
    {
        bool running = !Render::windowShouldClose();
        if ( running )
        {
            GLFWwindow * window = ( GLFWwindow * )( Render::getWindow() );
            if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
            {
                glfwSetWindowShouldClose( window, true );
            }
            glfwPollEvents();
        }
        return running;
    };

    static void handleInput( GLFWwindow * window, int key, int scancode, int action, int mods )
    {
        switch ( action )
        {
            case ( GLFW_PRESS ):
            {
                Input::press( key );
            }
            break;
            case ( GLFW_RELEASE ):
            {
                Input::release( key );
            }
            break;
        }
    }
}
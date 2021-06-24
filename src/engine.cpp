#include "assoc_array.hpp"
#include "color.hpp"
#include <cstdio>
#include "engine.hpp"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "input.hpp"
#include "render.hpp"
#include "unit.hpp"
#include "vector.hpp"

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

        Vector keys[ INPUT_MAX ];
        for ( int i = 0; i < INPUT_MAX; ++i )
        {
            keys[ i ] = vector_create( -1 );
        }
        vector_push( &keys[ 0 ], value_create_int( GLFW_KEY_RIGHT ) );
        vector_push( &keys[ 1 ], value_create_int( GLFW_KEY_LEFT ) );
        
        input_init( keys );

        for ( int i = 0; i < INPUT_MAX; ++i )
        {
            vector_destroy( &keys[ i ] );
        }

        if ( !render_init( "Boskeopolis World", Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS, { 0, 0, 0, 255 } ) )
        {
            printf( "¡Error! ¡Failed to initialize game renderer!\n" );
            return -1;
        }
        glfwSetKeyCallback( ( GLFWwindow * )( render_get_window() ), &handleInput );
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
        int running = !render_window_should_close();
        if ( running )
        {
            GLFWwindow * window = ( GLFWwindow * )( render_get_window() );
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
                input_press( key );
            }
            break;
            case ( GLFW_RELEASE ):
            {
                input_release( key );
            }
            break;
        }
    }
}
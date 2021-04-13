#include <cstdio>
#include "engine.hpp"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "input.hpp"
#include "render.hpp"

namespace Engine
{
    bool init()
    {
        glfwInit();
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
        glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
        Input::initKeys
        ({
            { Input::Key::RIGHT, { 0 } },
            { Input::Key::LEFT, { 1 } }
        });
        return true;
    };

    void close()
    {
        glfwTerminate();
    };

    int getTicks()
    {
        return 1;
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
}
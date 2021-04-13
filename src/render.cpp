#include "character.hpp"
#include "color.hpp"
#include <cstdio>
#include "filename.hpp"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "rect.hpp"
#include "render.hpp"
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Render
{
    static int magnification = 1;
    static int canvas_width = 0;
    static int canvas_height = 0;
    static GLFWwindow * window;
    static const char * vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec2 aPos;   // the position variable has attribute position 0\n"
        "layout (location = 1) in vec4 aColor; // the color variable has attribute position 1\n"
        "  \n"
        "out vec4 ourColor; // output a color to the fragment shader\n"
        "\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 ortho;\n"
        "uniform vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = ortho * view * model * vec4(aPos, 0.0, 1.0);\n"
        "    ourColor = color; // set ourColor to the input color we got from the vertex data\n"
        "}";

    static const char * fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;  \n"
        "in vec4 ourColor;\n"
        "  \n"
        "void main()\n"
        "{\n"
        "    FragColor = ourColor;\n"
        "}\n";


    static float vertices[] = {
         0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, // bottom right
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, // bottom left
        -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f  // top left 
    };
    static unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };  
    static unsigned int VAO;
    static unsigned int shaderProgram;

    static void drawBox( const Rect & rect, const Color & color );
    static void framebufferSizeCallback( GLFWwindow* window, int width, int height );

    bool init( const char * title, int width, int height, Color background )
    {
        canvas_width = width;
        canvas_height = height;
        window = glfwCreateWindow( width * magnification, height * magnification, title, nullptr, nullptr );
        if ( window == nullptr )
        {
            printf( "Failed to create GLFW window\n" );
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent( window );
        if ( !gladLoadGLLoader( ( GLADloadproc )( glfwGetProcAddress ) ) )
        {
            printf( "Failed to initialize GLAD\n" );
            return -1;
        }
        glViewport( 0, 0, width * magnification, height * magnification );

        // Vertex Shader
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        int  success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if( !success )
        {
            glGetShaderInfoLog( vertexShader, 512, NULL, infoLog );
            printf( "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog );
        }

        // Fragment Shader
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if( !success )
        {
            glGetShaderInfoLog( fragmentShader, 512, NULL, infoLog );
            printf( "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog );
        }

        // Shader Program
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Clean up shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);


        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            printf( "ERROR: Failed to create shader program\n%s\n", infoLog );
        }

        // VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // EBO
        unsigned int EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
        glEnableVertexAttribArray(1);

        glUseProgram(shaderProgram);

        glm::mat4 ortho = glm::ortho( 0.0f, 400.0f, 224.0f, 0.0f, -1.0f, 1.0f );
        unsigned int ortho_location = glGetUniformLocation(shaderProgram, "ortho");
        glUniformMatrix4fv( ortho_location, 1, GL_FALSE, glm::value_ptr(ortho));

        glfwSetFramebufferSizeCallback( window, framebufferSizeCallback );

        return true;
    };

    void close()
    {
        clearTextures();
    };

    void startUpdate()
    {
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );

        drawBox({ 0.0, 0.0, 400.0, 224.0 }, { 255, 255, 255, 255 });
        drawBox({ 200.0, 124.0, 8.0, 8.0 }, { 255, 0, 0, 255 });
        drawBox({ 384.0, -8.0, 32.0, 32.0 }, { 0, 0, 255, 255 });
    };

    void endUpdate()
    {
        glfwSwapBuffers( window );
    };

    void clearTextures()
    {
    };

    unsigned int getTextureId( const char * filename )
    {
        return 0;
    };

    void rect( const Rect & rect, const Color & color )
    {
    };

    void sprite( int texture_id, const Rect & src, const Rect & dest )
    {
    };

    void character( const Character & character )
    {
    };

    bool windowShouldClose()
    {
        return glfwWindowShouldClose( window );
    };

    void * getWindow()
    {
        return ( void * )( window );
    };

    void drawBox( const Rect & rect, const Color & color )
    {
        float r = ( float )( color.r ) / 255.0f;
        float g = ( float )( color.g ) / 255.0f;
        float b = ( float )( color.b ) / 255.0f;
        float a = ( float )( color.a ) / 255.0f;

        unsigned int color_location = glGetUniformLocation(shaderProgram, "color");
        glUniform4f(color_location, r, g, b, a );

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(rect.x + ( rect.w / 2.0f ), rect.y + ( rect.h / 2.0f ), 0.0f));
        unsigned int view_location = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv( view_location, 1, GL_FALSE, glm::value_ptr(view));

        auto identity = glm::mat4(1.0f);
        glm::mat4 model = identity;
        auto scale = glm::vec3( rect.w, rect.h, 0.0 );
        model = glm::scale( model, scale );
        unsigned int model_location = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv( model_location, 1, GL_FALSE, glm::value_ptr(model));
    
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    };

    void framebufferSizeCallback( GLFWwindow* window, int screen_width, int screen_height )
    {
        double screen_aspect_ratio = ( double )( canvas_width / canvas_height );
        double monitor_aspect_ratio = ( double )( screen_width ) / ( double )( screen_height );

        magnification = std::max(
            ( int )( floor(
                ( monitor_aspect_ratio > screen_aspect_ratio )
                    ? ( double )( screen_height ) / ( double )( canvas_height )
                    : ( double )( screen_width ) / ( double )( canvas_width )
            )),
            1
        );

        GLint magnified_canvas_width = canvas_width * magnification;
        GLint magnified_canvas_height = canvas_height * magnification;
        GLint x = ( int )( floor( ( double )( screen_width - magnified_canvas_width ) / 2.0 ) );
        GLint y = ( int )( floor( ( double )( screen_height - magnified_canvas_height ) / 2.0 ) );
        glViewport( x, y, magnified_canvas_width, magnified_canvas_height );
    }
}
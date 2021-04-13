#include "character.hpp"
#include "color.hpp"
#include <cstdio>
#include "filename.hpp"
#include <fstream>
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

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>


namespace Render
{
    static int magnification = 1;
    static int canvas_width = 0;
    static int canvas_height = 0;
    static GLFWwindow * window;

    static float vertices[] = {
        // Vertices     // Texture coords
         0.5f,  0.5f,   1.0f, 1.0f, // top right
         0.5f, -0.5f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f,   0.0f, 1.0f  // top left 
    };

    static unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    static unsigned int VAO;
    static unsigned int sprite_shader, rect_shader;
    static unsigned int texture;
    static int texture_width, texture_height;

    static void drawBox( const Rect & rect, const Color & color );
    static void drawSprite( const Rect & src, const Rect & dest );
    static void framebufferSizeCallback( GLFWwindow* window, int width, int height );
    static unsigned int generateShaderProgram( std::vector<const char *> vertex_shaders, std::vector<const char *> fragment_shaders );
    static unsigned int generateShader( GLenum type, const char * file );
    static std::string loadShader( const char * local );

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

        sprite_shader = generateShaderProgram({ "vertex" }, { "sprite" });
        rect_shader = generateShaderProgram({ "vertex" }, { "rect" });

        int texture_channels;
        unsigned char * texture_data = stbi_load( "assets/graphics/sprites/autumn.png", &texture_width, &texture_height, &texture_channels, STBI_rgb_alpha );
        if ( texture_data == nullptr )
        {
            printf( "Error loading texture.\n" );
            return false;
        }
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        stbi_image_free(texture_data);

        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2* sizeof(float)));
        glEnableVertexAttribArray(1);

        for ( unsigned int shader : { sprite_shader, rect_shader } )
        {
            glUseProgram(shader);
            glm::mat4 ortho = glm::ortho( 0.0f, 400.0f, 224.0f, 0.0f, -1.0f, 1.0f );
            unsigned int ortho_location = glGetUniformLocation(shader, "ortho");
            glUniformMatrix4fv( ortho_location, 1, GL_FALSE, glm::value_ptr(ortho));
        }

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
        drawSprite({ 48.0, 30.0, 16.0, 21.0 }, { 200.0, 100.0, 16.0, 21.0 });
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
        glUseProgram(rect_shader);
        float r = ( float )( color.r ) / 255.0f;
        float g = ( float )( color.g ) / 255.0f;
        float b = ( float )( color.b ) / 255.0f;
        float a = ( float )( color.a ) / 255.0f;

        unsigned int color_location = glGetUniformLocation(rect_shader, "color");
        glUniform4f(color_location, r, g, b, a );

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2* sizeof(float)));
        glEnableVertexAttribArray(1);

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(rect.x + ( rect.w / 2.0f ), rect.y + ( rect.h / 2.0f ), 0.0f));
        unsigned int view_location = glGetUniformLocation(rect_shader, "view");
        glUniformMatrix4fv( view_location, 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale( model, glm::vec3( rect.w, rect.h, 0.0 ) );
        unsigned int model_location = glGetUniformLocation(rect_shader, "model");
        glUniformMatrix4fv( model_location, 1, GL_FALSE, glm::value_ptr(model));
    
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    };

    void drawSprite( const Rect & src, const Rect & dest )
    {
        glUseProgram(sprite_shader);
        // Src Coords
        vertices[ 14 ] = vertices[ 10 ] = 1.0f / ( float )( texture_width ) * src.x; // Left X
        vertices[ 2 ] = vertices[ 6 ] = 1.0f / ( float )( texture_width ) * ( src.x + src.w );  // Right X
        vertices[ 15 ] = vertices[ 3 ] = 1.0f / ( float )( texture_height ) * ( src.y + src.h ); // Top Y
        vertices[ 11 ] = vertices[ 7 ] = 1.0f / ( float )( texture_height ) * src.y;  // Bottom Y

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2* sizeof(float)));
        glEnableVertexAttribArray(1);

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3( dest.x + ( dest.w / 2.0f ), dest.y + ( dest.h / 2.0f ), 0.0f));
        unsigned int view_location = glGetUniformLocation(sprite_shader, "view");
        glUniformMatrix4fv( view_location, 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale( model, glm::vec3( dest.w, dest.h, 0.0 ) );
        unsigned int model_location = glGetUniformLocation(sprite_shader, "model");
        glUniformMatrix4fv( model_location, 1, GL_FALSE, glm::value_ptr(model));
    
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
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

    static unsigned int generateShaderProgram( std::vector<const char *> vertex_shaders, std::vector<const char *> fragment_shaders )
    {
        // Shader Program
        unsigned int program = glCreateProgram();

        std::vector<unsigned int> shaders;
        for ( auto shader : vertex_shaders )
        {
            shaders.emplace_back( generateShader( GL_VERTEX_SHADER, shader ) );
        }
        for ( auto shader : fragment_shaders )
        {
            shaders.emplace_back( generateShader( GL_FRAGMENT_SHADER, shader ) );
        }

        for ( auto shader : shaders )
        {
            glAttachShader( program, shader );
        }

        glLinkProgram(program);

        int  success;
        char infoLog[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            printf( "ERROR: Failed to create shader program\n%s\n", infoLog );
        }

        // Clean up shaders.
        for ( auto shader : shaders )
        {
            glDeleteShader( shader );
        }

        return program;
    }

    static unsigned int generateShader( GLenum type, const char * file )
    {
        int success;
        char infoLog[ 512 ];
        unsigned int shader;
        shader = glCreateShader( type );
        std::string string = loadShader( file );
        const char * cstring = string.c_str();
        glShaderSource( shader, 1, &cstring, NULL );
        glCompileShader( shader );
        glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
        if( !success )
        {
            glGetShaderInfoLog( shader, 512, NULL, infoLog );
            printf( "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog );
        }
        return shader;
    }

    static std::string loadShader( const char * local )
    {
        const std::string full_filename = Filename::shader( local );
        std::ifstream ifs( full_filename );
        if( !ifs.is_open() )
        {
            throw std::runtime_error( "Failed to open shaders." );
        }
        std::string content{
            (std::istreambuf_iterator<char>(ifs) ),
            (std::istreambuf_iterator<char>()    )
        };
        ifs.close();
        return content;
    };
}
#include "character.hpp"
#include "color.hpp"
#include <cstdio>
#include "filename.hpp"
#include <fstream>
#include "game_state_machine.hpp"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "graphic.hpp"
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

struct Texture
{
    int width;
    int height;
};

namespace Render
{
    static constexpr int MAX_TEXTURES = 200;
    static constexpr int VERTEX_SIZE = 8;
    static constexpr int MAX_GRAPHICS = 512;

    static int magnification = 3;
    static int canvas_width = 0;
    static int canvas_height = 0;
    static float vertices[] = {
        // Vertices     // Texture coords   // Color
         0.5f,  0.5f,   1.0f, 1.0f,         1.0f, 1.0f, 1.0f, 1.0f,// top right
         0.5f, -0.5f,   1.0f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f, // bottom right
        -0.5f, -0.5f,   0.0f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f, // bottom left
        -0.5f,  0.5f,   0.0f, 1.0f,         1.0f, 1.0f, 1.0f, 1.0f,  // top left 
    };
    static unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    static unsigned int VAO;
    static unsigned int sprite_shader, rect_shader, text_shader;
    static unsigned int texture_ids[ MAX_TEXTURES ];
    static Texture textures[ MAX_TEXTURES ];
    static std::unordered_map<const char *, int> texture_map;
    static unsigned int number_of_textures = 0;
    static unsigned int palette_texture;
    static unsigned int text_texture;
    static int number_of_graphics = 0;
    static int graphics_per_layer[ GameStateMachine::MAX_STATES ][ Unit::NUMBER_OF_LAYERS ];
    static int number_of_states = 0;
    static unsigned int graphics_ids_used = 0;
    static std::unordered_map<int, int> graphics_map;
    static Graphic layers[ MAX_GRAPHICS ];
    static GLFWwindow * window;

    static void drawBox( const Rect & rect, const Color & top_left_color, const Color & top_right_color, const Color & bottom_left_color, const Color & bottom_right_color );
    static void framebufferSizeCallback( GLFWwindow* window, int width, int height );
    static unsigned int generateShaderProgram( std::vector<const char *> vertex_shaders, std::vector<const char *> fragment_shaders );
    static unsigned int generateShader( GLenum type, const char * file );
    static std::string loadShader( const char * local );
    static void bufferVertices();
    static void rect( const Rect & rect, const Color & color );
    static void sprite( unsigned int texture_id, unsigned int palette_id, const Rect & src, const Rect & dest, bool flip_x, bool flip_y, float rotation_x, float rotation_y, float rotation_z );
    static void character( const Character & character, const Color & color );

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
        text_shader = generateShaderProgram({ "vertex" }, { "text" });

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glGenTextures( MAX_TEXTURES, texture_ids );

        palette_texture = getTextureId( "sprites/palette.png", false );
        text_texture = getTextureId( "text/latin.png", false );

        for ( unsigned int shader : { sprite_shader, rect_shader, text_shader } )
        {
            glUseProgram(shader);
            glm::mat4 ortho = glm::ortho( 0.0f, 400.0f, 224.0f, 0.0f, -1.0f, 1.0f );
            unsigned int ortho_location = glGetUniformLocation(shader, "ortho");
            glUniformMatrix4fv( ortho_location, 1, GL_FALSE, glm::value_ptr(ortho));
        }

        glfwSetFramebufferSizeCallback( window, framebufferSizeCallback );

        clearGraphics();


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
        bufferVertices();

        return true;
    };

    void close()
    {
        glDeleteTextures( MAX_TEXTURES, texture_ids );
    };

    void update()
    {
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        for ( int i = 0; i < number_of_graphics; ++i )
        {
            switch ( layers[ i ].type )
            {
                case ( Graphic::Type::RECT ):
                {
                    rect( layers[ i ].data.rect.rect, layers[ i ].data.rect.color );
                }
                break;
                case ( Graphic::Type::TEXT ):
                {
                    for ( int c = 0; c < layers[ i ].data.text.number_of_characters; ++c )
                    {
                        character( layers[ i ].data.text.characters[ c ], layers[ i ].data.text.color );
                    }
                }
                break;
                case ( Graphic::Type::SPRITE ):
                {
                    sprite
                    (
                        layers[ i ].data.sprite.texture,
                        layers[ i ].data.sprite.palette,
                        layers[ i ].data.sprite.src,
                        layers[ i ].data.sprite.dest,
                        layers[ i ].data.sprite.flip_x,
                        layers[ i ].data.sprite.flip_y,
                        layers[ i ].data.sprite.rotation_x,
                        layers[ i ].data.sprite.rotation_y,
                        layers[ i ].data.sprite.rotation_z
                    );
                }
                break;
            }
        }
        glfwSwapBuffers( window );
    };

    void clearTextures()
    {
        number_of_textures = 2;
    };

    unsigned int getTextureId( const char * local, bool indexed )
    {
        auto seek = texture_map.find( local );
        if ( seek != texture_map.end() )
        {
            return seek->second;
        }

        int texture_channels;
        std::string full_filename = Filename::image( local );
        unsigned char * texture_data = stbi_load( full_filename.c_str(), &textures[ number_of_textures ].width, &textures[ number_of_textures ].height, &texture_channels, STBI_rgb_alpha );
        if ( texture_data == nullptr )
        {
            throw std::runtime_error( "Couldn’t load texture file." );
        }
        glBindTexture(GL_TEXTURE_2D, texture_ids[ number_of_textures ] );
        glTexImage2D(GL_TEXTURE_2D, 0, ( indexed ) ? GL_R8 : GL_RGBA, textures[ number_of_textures ].width, textures[ number_of_textures ].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        stbi_image_free(texture_data);
        texture_map.insert( std::pair<const char *, int> ( local, number_of_textures ) );
        return number_of_textures++;
    };

    void setNumberOfStates( int number )
    {
        number_of_states = number;
    };

    void clearGraphics()
    {
        number_of_graphics = 0;
        for ( int i = 0; i < GameStateMachine::MAX_STATES; ++i )
        {
            for ( int j = 0; j < Unit::NUMBER_OF_LAYERS; ++j )
            {
                graphics_per_layer[ i ][ j ] = 0;
            }
        }
    };

    void clearStateGraphics( int state )
    {
        // Count up filled layers till current state.
        int i = 0;
        for ( int si = 0; si < state; ++si )
        {
            for ( int li = 0; li < Unit::NUMBER_OF_LAYERS; ++li )
            {
                i += graphics_per_layer[ si ][ li ];
            }
        }

        // For every layer o’ current state, decrease # o’ graphics per layer & set graphics per layer to 0.
        for ( int li = 0; li <= Unit::NUMBER_OF_LAYERS; ++li )
        {
            number_of_graphics -= graphics_per_layer[ state ][ li ];
            graphics_per_layer[ state ][ li ] = 0;
        }

        // TODO: If it’s possible to clear graphics o’ state that isn’t @ the top,
        // move later states’ graphics down to next blank space.
    };

    bool windowShouldClose()
    {
        return glfwWindowShouldClose( window );
    };

    void * getWindow()
    {
        return ( void * )( window );
    };

    void drawBox( const Rect & rect, const Color & top_left_color, const Color & top_right_color, const Color & bottom_left_color, const Color & bottom_right_color )
    {
        glUseProgram(rect_shader);

        vertices[ 4 ] = bottom_right_color.r / 255.0f;
        vertices[ 5 ] = bottom_right_color.g / 255.0f;
        vertices[ 6 ] = bottom_right_color.b / 255.0f;
        vertices[ 7 ] = bottom_right_color.a / 255.0f;

        vertices[ 4 + VERTEX_SIZE ] = top_right_color.r / 255.0f;
        vertices[ 5 + VERTEX_SIZE ] = top_right_color.g / 255.0f;
        vertices[ 6 + VERTEX_SIZE ] = top_right_color.b / 255.0f;
        vertices[ 7 + VERTEX_SIZE ] = top_right_color.a / 255.0f;

        vertices[ 4 + VERTEX_SIZE * 2 ] = top_left_color.r / 255.0f;
        vertices[ 5 + VERTEX_SIZE * 2 ] = top_left_color.g / 255.0f;
        vertices[ 6 + VERTEX_SIZE * 2 ] = top_left_color.b / 255.0f;
        vertices[ 7 + VERTEX_SIZE * 2 ] = top_left_color.a / 255.0f;

        vertices[ 4 + VERTEX_SIZE * 3 ] = bottom_left_color.r / 255.0f;
        vertices[ 5 + VERTEX_SIZE * 3 ] = bottom_left_color.g / 255.0f;
        vertices[ 6 + VERTEX_SIZE * 3 ] = bottom_left_color.b / 255.0f;
        vertices[ 7 + VERTEX_SIZE * 3 ] = bottom_left_color.a / 255.0f;

        bufferVertices();

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

    unsigned int addGraphic( Graphic gfx, int state, Unit::Layer layer )
    {
        // Count up graphics 
        int i = 0;
        for ( int si = 0; si < state; ++si )
        {
            for ( int li = 0; li < Unit::NUMBER_OF_LAYERS; ++li )
            {
                i += graphics_per_layer[ si ][ li ];
            }
        }
        for ( int li = 0; li <= ( int )( layer ); ++li )
        {
            i += graphics_per_layer[ state ][ li ];
        }
        for ( int gi = number_of_graphics - 1; gi > i; --gi )
        {
            for ( auto it = graphics_map.begin(); it != graphics_map.end(); ++it )
            {
                if ( it->second == gi )
                {
                    ++it->second;
                    break;
                }
            }
            layers[ gi + 1 ] = layers[ gi ];
        }
        layers[ i ] = gfx;
        graphics_map.insert( std::pair<int, int> ( graphics_ids_used, i ) );
        ++number_of_graphics;
        ++graphics_per_layer[ state ][ ( int )( layer ) ];
        return graphics_ids_used++;
    };

    Graphic & getGraphic( unsigned int id )
    {
        auto seek = graphics_map.find( id );
        if ( seek == graphics_map.end() )
        {
            throw std::runtime_error( "Could not find graphic." );
        }
        return layers[ seek->second ];
    };

    static void rect( const Rect & rect, const Color & color )
    {
        drawBox( rect, color, color, color, color );
    };

    static void sprite( unsigned int texture_id, unsigned int palette_id, const Rect & src, const Rect & dest, bool flip_x, bool flip_y, float rotation_x, float rotation_y, float rotation_z )
    {
        glUseProgram(sprite_shader);

        // Src Coords
        if ( flip_x )
        {
            vertices[ 2 ] = vertices[ 2 + VERTEX_SIZE ] = 1.0f / ( float )( textures[ texture_id ].width ) * src.x; // Left X
            vertices[ 2 + VERTEX_SIZE * 3 ] = vertices[ 2 + VERTEX_SIZE * 2 ] = 1.0f / ( float )( textures[ texture_id ].width ) * ( src.x + src.w );  // Right X
        }
        else
        {
            vertices[ 2 + VERTEX_SIZE * 3 ] = vertices[ 2 + VERTEX_SIZE * 2 ] = 1.0f / ( float )( textures[ texture_id ].width ) * src.x; // Left X
            vertices[ 2 ] = vertices[ 2 + VERTEX_SIZE ] = 1.0f / ( float )( textures[ texture_id ].width ) * ( src.x + src.w );  // Right X
        }

        if ( flip_y )
        {
            vertices[ 3 + VERTEX_SIZE * 2 ] = vertices[ 3 + VERTEX_SIZE ] = 1.0f / ( float )( textures[ texture_id ].height ) * ( src.y + src.h ); // Top Y
            vertices[ 3 + VERTEX_SIZE * 3 ] = vertices[ 3 ] = 1.0f / ( float )( textures[ texture_id ].height ) * src.y;  // Bottom Y
        }
        else
        {
            vertices[ 3 + VERTEX_SIZE * 3 ] = vertices[ 3 ] = 1.0f / ( float )( textures[ texture_id ].height ) * ( src.y + src.h ); // Top Y
            vertices[ 3 + VERTEX_SIZE * 2 ] = vertices[ 3 + VERTEX_SIZE ] = 1.0f / ( float )( textures[ texture_id ].height ) * src.y;  // Bottom Y
        }

        bufferVertices();

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3( dest.x + ( dest.w / 2.0f ), dest.y + ( dest.h / 2.0f ), 0.0f));
        unsigned int view_location = glGetUniformLocation(sprite_shader, "view");
        glUniformMatrix4fv( view_location, 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale( model, glm::vec3( dest.w, dest.h, 0.0 ) );
        model = glm::rotate( model, glm::radians( rotation_z ), glm::vec3( 0.0, 0.0, 1.0 ) );
        model = glm::rotate( model, glm::radians( rotation_y ), glm::vec3( 0.0, 1.0, 0.0 ) );
        model = glm::rotate( model, glm::radians( rotation_x ), glm::vec3( 1.0, 0.0, 0.0 ) );
        unsigned int model_location = glGetUniformLocation(sprite_shader, "model");
        glUniformMatrix4fv( model_location, 1, GL_FALSE, glm::value_ptr(model));
    
        GLint palette_id_location = glGetUniformLocation(sprite_shader, "palette_id");
        glUniform1f( palette_id_location, ( float )( palette_id ) );

        GLint texture_data_location = glGetUniformLocation(sprite_shader, "texture_data");
        GLint palette_data_location = glGetUniformLocation(sprite_shader, "palette_data");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_ids[ texture_id ] );
        glUniform1i(texture_data_location, 0);
        glActiveTexture(GL_TEXTURE1 );
        glBindTexture(GL_TEXTURE_2D, texture_ids[ palette_texture ] );
        glUniform1i(palette_data_location, 1);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    };

    static void character( const Character & character, const Color & color )
    {
        glUseProgram(text_shader);

        // Src Coords
            vertices[ 2 + VERTEX_SIZE * 3 ] = vertices[ 2 + VERTEX_SIZE * 2 ] = 1.0f / ( float )( textures[ text_texture ].width ) * character.src_x; // Left X
            vertices[ 2 ] = vertices[ 2 + VERTEX_SIZE ] = 1.0f / ( float )( textures[ text_texture ].width ) * ( character.src_x + character.w );  // Right X
            vertices[ 3 + VERTEX_SIZE * 3 ] = vertices[ 3 ] = 1.0f / ( float )( textures[ text_texture ].height ) * ( character.src_y + character.h ); // Top Y
            vertices[ 3 + VERTEX_SIZE * 2 ] = vertices[ 3 + VERTEX_SIZE ] = 1.0f / ( float )( textures[ text_texture ].height ) * character.src_y;  // Bottom Y

        bufferVertices();

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3( character.dest_x + ( character.w / 2.0f ), character.dest_y + ( character.h / 2.0f ), 0.0f));
        unsigned int view_location = glGetUniformLocation(text_shader, "view");
        glUniformMatrix4fv( view_location, 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale( model, glm::vec3( character.w, character.h, 0.0 ) );
        unsigned int model_location = glGetUniformLocation(text_shader, "model");
        glUniformMatrix4fv( model_location, 1, GL_FALSE, glm::value_ptr(model));

        unsigned int color_location = glGetUniformLocation(text_shader, "color");
        glUniform4f( color_location, color.r, color.g, color.b, color.a );

        GLint texture_data_location = glGetUniformLocation(text_shader, "texture_data");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_ids[ text_texture ] );
        glUniform1i(texture_data_location, 0);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    };

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

    static void bufferVertices()
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), (void*)(2* sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), (void*)(4* sizeof(float)));
        glEnableVertexAttribArray(2);
    }
}
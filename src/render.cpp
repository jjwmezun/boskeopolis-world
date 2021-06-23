#include "character.hpp"
#include "color.hpp"
#include <stdio.h>
#include "filename.hpp"
#include "game_state_machine.hpp"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "graphic.hpp"
#include "io.hpp"
#include "log.hpp"
#include "rect.hpp"
#include "render.hpp"

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

static constexpr int MAX_TEXTURES = 200;
static constexpr int VERTEX_SIZE = 8;
static constexpr int MAX_GRAPHICS = 512;
static constexpr int TEXTURE_MAP_CAPACITY = MAX_TEXTURES * 1.25;

typedef uint32_t hash_t;
typedef struct { char * string; hash_t hash; } TextureMapKey;
typedef struct { TextureMapKey key; unsigned int value; } TextureMapEntry;

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
static TextureMapEntry texture_map[ TEXTURE_MAP_CAPACITY ];
static unsigned int number_of_textures = 0;
static unsigned int palette_texture;
static unsigned int text_texture;
static unsigned int number_of_graphics = 0;
static int graphics_per_layer[ GameStateMachine::MAX_STATES ][ Unit::NUMBER_OF_LAYERS ];
static int number_of_states = 0;
static int graphics_map[ MAX_GRAPHICS ];
static Graphic layers[ MAX_GRAPHICS ];
static GLFWwindow * window;

static void draw_box( const Rect & rect, const Color & top_left_color, const Color & top_right_color, const Color & bottom_left_color, const Color & bottom_right_color );
static void framebuffer_size_callback( GLFWwindow* window, int width, int height );
static unsigned int generate_shader_program( const char * vertex_shader, const char * fragment_shader );
static unsigned int generate_shader( GLenum type, const char * file );
static char * load_shader( const char * local );
static void buffer_vertices();
static void rect( const Rect & rect, const Color & color );
static void sprite( unsigned int texture_id, unsigned int palette_id, const Rect & src, const Rect & dest, bool flip_x, bool flip_y, float rotation_x, float rotation_y, float rotation_z );
static void character( const Character & character, const Color & color );

static TextureMapEntry * hash_find_entry( const char * needle_string, hash_t needle_hash );
static uint32_t hash_string( const char * key );

bool render_init( const char * title, int width, int height, Color background )
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

    sprite_shader = generate_shader_program( "vertex", "sprite" );
    rect_shader = generate_shader_program( "vertex", "rect" );
    text_shader = generate_shader_program( "vertex", "text" );

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenTextures( MAX_TEXTURES, texture_ids );

    palette_texture = render_get_texture_id( "sprites/palette.png", false );
    text_texture = render_get_texture_id( "text/latin.png", false );

    for ( unsigned int shader : { sprite_shader, rect_shader, text_shader } )
    {
        glUseProgram(shader);
        glm::mat4 ortho = glm::ortho( 0.0f, 400.0f, 224.0f, 0.0f, -1.0f, 1.0f );
        unsigned int ortho_location = glGetUniformLocation(shader, "ortho");
        glUniformMatrix4fv( ortho_location, 1, GL_FALSE, glm::value_ptr(ortho));
    }

    glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );

    render_clear_graphics();

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
    buffer_vertices();

    for ( int i = 0; i < TEXTURE_MAP_CAPACITY; ++i )
    {
        texture_map[ i ] = { { NULL, 0 }, 0 };
    }

    for ( int i = 0; i < MAX_GRAPHICS; ++i )
    {
        graphics_map[ i ] = -1;
    }

    return true;
};

void render_close()
{
    glDeleteTextures( MAX_TEXTURES, texture_ids );
};

void render_update()
{
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );
    for ( unsigned int i = 0; i < number_of_graphics; ++i )
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

void render_clear_textures()
{
    number_of_textures = 2;
};

unsigned int render_get_texture_id( const char * local, bool indexed )
{
    const hash_t needle_hash = hash_string( local );
    TextureMapEntry * entry = hash_find_entry( local, needle_hash );
    if ( entry->key.string != NULL )
    {
        return entry->value;
    }
    entry->key.string = ( char * )( malloc( strlen( local ) + 1 ) );
    strcpy( entry->key.string, local );
    entry->key.hash = needle_hash;
    entry->value = number_of_textures;

    int texture_channels;
    char * full_filename = filename_image( local );
    unsigned char * texture_data = stbi_load( full_filename, &textures[ number_of_textures ].width, &textures[ number_of_textures ].height, &texture_channels, STBI_rgb_alpha );
    free( full_filename );
    if ( texture_data == nullptr )
    {
        log_error( "Couldn’t load texture file." );
    }
    glBindTexture(GL_TEXTURE_2D, texture_ids[ number_of_textures ] );
    glTexImage2D(GL_TEXTURE_2D, 0, ( indexed ) ? GL_R8 : GL_RGBA, textures[ number_of_textures ].width, textures[ number_of_textures ].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(texture_data);

    return number_of_textures++;
};

void render_set_states_number( int number )
{
    number_of_states = number;
};

void render_clear_graphics()
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

void render_clear_state_graphics( int state )
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

    // Find all removed graphics in graphics map & nullify ( set to -1 ).
    while ( i < ( int )( number_of_graphics ) )
    {
        for ( unsigned int j = 0; j < number_of_graphics; ++j )
        {
            if ( graphics_map[ j ] == i )
            {
                graphics_map[ j ] = -1;
                break;
            }
        }
        ++i;
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

bool render_window_should_close()
{
    return glfwWindowShouldClose( window );
};

void * render_get_window()
{
    return ( void * )( window );
};

void draw_box( const Rect & rect, const Color & top_left_color, const Color & top_right_color, const Color & bottom_left_color, const Color & bottom_right_color )
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

    buffer_vertices();

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

void framebuffer_size_callback( GLFWwindow* window, int screen_width, int screen_height )
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

unsigned int render_add_graphic( Graphic gfx, int state, Unit::Layer layer )
{
    // Count up graphics to where current graphic should be.
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

    // Move all graphics ’bove where current show go forward 1.
    for ( int gi = number_of_graphics - 1; gi > i; --gi )
    {
        // Find index in graphic map & increment so they still match.
        for ( unsigned int mi = 0; mi != number_of_graphics; ++mi )
        {
            if ( graphics_map[ mi ] == gi )
            {
                ++graphics_map[ mi ];
                break;
            }
        }
        layers[ gi + 1 ] = layers[ gi ];
    }

    // Set current graphic.
    layers[ i ] = gfx;

    unsigned int map_index = 0;
    while ( map_index < number_of_graphics )
    {
        if ( graphics_map[ map_index ] == -1 )
        {
            break;
        }
        ++map_index;
    }
    graphics_map[ map_index ] = i;
    ++graphics_per_layer[ state ][ ( int )( layer ) ];
    ++number_of_graphics;
    return map_index;
};

Graphic & render_get_graphic( unsigned int id )
{
    return layers[ graphics_map[ id ] ];
};

static void rect( const Rect & rect, const Color & color )
{
    draw_box( rect, color, color, color, color );
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

    buffer_vertices();

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

    buffer_vertices();

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

static unsigned int generate_shader_program( const char * vertex_shader, const char * fragment_shader )
{
    unsigned int program = glCreateProgram();
    const unsigned int vertex_shader_id = generate_shader( GL_VERTEX_SHADER, vertex_shader );
    const unsigned int fragment_shader_id = generate_shader( GL_FRAGMENT_SHADER, fragment_shader );
    glAttachShader( program, vertex_shader_id );
    glAttachShader( program, fragment_shader_id );

    glLinkProgram( program );

    int  success;
    char infoLog[ 512 ];
    glGetProgramiv( program, GL_LINK_STATUS, &success );
    if( !success )
    {
        glGetProgramInfoLog( program, 512, NULL, infoLog );
        printf( "ERROR: Failed to create shader program\n%s\n", infoLog );
    }

    glDeleteShader( vertex_shader_id );
    glDeleteShader( fragment_shader_id );

    return program;
}

static unsigned int generate_shader( GLenum type, const char * file )
{
    int success;
    char infoLog[ 512 ];
    unsigned int shader;
    shader = glCreateShader( type );
    char * string = load_shader( file );
    glShaderSource( shader, 1, &string, NULL );
    free( string );
    glCompileShader( shader );
    glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
    if( !success )
    {
        glGetShaderInfoLog( shader, 512, NULL, infoLog );
        printf( "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog );
    }
    return shader;
}

static char * load_shader( const char * local )
{
    char * full_filename = filename_shader( local );
    char * content = io_read( full_filename );
    free( full_filename );
    return content;
};

static void buffer_vertices()
{
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), (void*)(2* sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), (void*)(4* sizeof(float)));
    glEnableVertexAttribArray(2);
}

static TextureMapEntry * hash_find_entry( const char * needle_string, hash_t needle_hash )
{
    while ( true )
    {
        TextureMapEntry * entry = &texture_map[ needle_hash ];
        if ( entry->key.string == NULL || strcmp( entry->key.string, needle_string ) == 0 )
        {
            return entry;
        }
        needle_hash = ( needle_hash + 1 ) % TEXTURE_MAP_CAPACITY;
    }
}

static uint32_t hash_string( const char * key )
{
    uint32_t hash = 2166136261u;
    const int length = strlen( key );
    for ( int i = 0; i < length; i++ )
    {
        hash ^= ( uint8_t )( key[ i ] );
        hash *= 16777619;
    }
    return hash % TEXTURE_MAP_CAPACITY;
}
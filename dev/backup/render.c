#include "character.h"
#include "filename.h"
#include "game_state_machine.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "input.h"
#include "io.h"
#include "log.h"
#include <math.h>
#include "rect.h"
#include "render.h"
#include <stdint.h>

#include <cglm/cglm.h>
#include <cglm/call.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

typedef struct
{
    int width;
    int height;
}
Texture;

#define MAX_TEXTURES 200
#define VERTEX_SIZE 8
#define MAX_GRAPHICS 512
#define TEXTURE_MAP_CAPACITY 250

#define BASE_MATRIX {\
    { 1.0f, 0.0f, 0.0f, 0.0f },\
    { 0.0f, 1.0f, 0.0f, 0.0f },\
    { 0.0f, 0.0f, 1.0f, 0.0f },\
    { 0.0f, 0.0f, 0.0f, 1.0f }\
}

typedef uint32_t render_hash_t;
typedef struct { char * string; render_hash_t hash; } TextureMapKey;
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
static unsigned int sprite_shader, rect_shader, text_shader, tilemap_shader;
static unsigned int texture_ids[ MAX_TEXTURES ];
static Texture textures[ MAX_TEXTURES ];
static TextureMapEntry texture_map[ TEXTURE_MAP_CAPACITY ];
static unsigned int number_of_textures = 0;
static unsigned int palette_texture;
static unsigned int text_texture;
static unsigned int number_of_graphics = 0;
static int graphics_per_layer[ MAX_STATES ][ NUMBER_OF_LAYERS ];
static int number_of_states = 0;
static int graphics_map[ MAX_GRAPHICS ];
static Graphic layers[ MAX_GRAPHICS ];
static GLFWwindow * window;
static Rect camera = { 0.0f, 0.0f, WINDOW_WIDTH_PIXELS, WINDOW_HEIGHT_PIXELS };

static void draw_box( const Rect * rect, const Color * top_left_color, const Color * top_right_color, const Color * bottom_left_color, const Color * bottom_right_color );
static void framebuffer_size_callback( GLFWwindow* window, int width, int height );
static unsigned int generate_shader_program( const char * vertex_shader, const char * fragment_shader );
static unsigned int generate_shader( GLenum type, const char * file );
static char * load_shader( const char * local );
static void buffer_vertices();
static void sprite( unsigned int texture_id, unsigned int palette_id, const Rect * src, const Rect * dest, int flip_x, int flip_y, float rotation_x, float rotation_y, float rotation_z );
static void character( const Character * character, const Color * color );
static unsigned int render_get_texture_id_generic( const char * local, int indexed );
static void set_vertices_colors( const Color * top_left_color, const Color * top_right_color, const Color * bottom_left_color, const Color * bottom_right_color );
static void set_vertices_view( float x, float y );
static void render_vertices();

static TextureMapEntry * hash_find_entry( const char * needle_string, render_hash_t needle_hash );
static uint32_t hash_string( const char * key );

int render_init()
{
    canvas_width = WINDOW_WIDTH_PIXELS;
    canvas_height = WINDOW_HEIGHT_PIXELS;
    window = glfwCreateWindow( canvas_width * magnification, canvas_height * magnification, "Boskeopolis World", NULL, NULL );
    if ( window == NULL )
    {
        log_error( "Failed to create GLFW window\n" );
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent( window );
    if ( !gladLoadGLLoader( ( GLADloadproc )( glfwGetProcAddress ) ) )
    {
        log_error( "Failed to initialize GLAD\n" );
        return -1;
    }
    glViewport( 0, 0, canvas_width * magnification, canvas_height * magnification );

    sprite_shader = generate_shader_program( "vertex", "sprite" );
    rect_shader = generate_shader_program( "vertex", "rect" );
    text_shader = generate_shader_program( "vertex", "text" );
    tilemap_shader = generate_shader_program( "vertex", "tilemap" );

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenTextures( MAX_TEXTURES, texture_ids );

    palette_texture = render_get_texture_id_noindex( "sprites/palette.png" );
    text_texture = render_get_texture_id_noindex( "text/latin.png" );

    unsigned int shaders[ 4 ] = { sprite_shader, rect_shader, text_shader, tilemap_shader };
    for ( int i = 0; i < 4; ++i )
    {
        unsigned int shader = shaders[ i ];
        glUseProgram(shader);

        mat4 ortho = {
            { 1.0f, 1.0f, 1.0f, 1.0f },
            { 1.0f, 1.0f, 1.0f, 1.0f },
            { 1.0f, 1.0f, 1.0f, 1.0f },
            { 1.0f, 1.0f, 1.0f, 1.0f }
        };
        glm_ortho_rh_no( 0.0f, 400.0f, 224.0f, 0.0f, -1.0f, 1.0f, ortho );
        unsigned int ortho_location = glGetUniformLocation( shader, "ortho" );
        glUniformMatrix4fv( ortho_location, 1, GL_FALSE, ( float * )( ortho ) );
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

    memset( texture_map, 0, sizeof( TextureMapEntry ) * TEXTURE_MAP_CAPACITY );

    for ( int i = 0; i < MAX_GRAPHICS; ++i )
    {
        graphics_map[ i ] = -1;
    }

    return 1;
};

void render_close()
{
    for ( int i = 0; i < TEXTURE_MAP_CAPACITY; ++i )
    {
        if ( texture_map[ i ].key.string != NULL )
        {
            free( texture_map[ i ].key.string );
        }
    }
    glDeleteTextures( MAX_TEXTURES, texture_ids );
};

void render_update()
{
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );
    for ( unsigned int i = 0; i < number_of_graphics; ++i )
    {
        // Adjust camera for this graphic.
        unsigned int shaders[ 4 ] = { sprite_shader, rect_shader, text_shader, tilemap_shader };
        Rect c = { 0.0f, 0.0f, WINDOW_WIDTH_PIXELS, WINDOW_HEIGHT_PIXELS };
        if ( !layers[ i ].abs )
        {
            c = camera;
        }
        for ( unsigned int j = 0; j < 4; ++j )
        {
            unsigned int shader = shaders[ j ];
            glUseProgram( shader );
            mat4 ortho =
            {
                { 1.0f, 1.0f, 1.0f, 1.0f },
                { 1.0f, 1.0f, 1.0f, 1.0f },
                { 1.0f, 1.0f, 1.0f, 1.0f },
                { 1.0f, 1.0f, 1.0f, 1.0f }
            };
            glm_ortho_rh_no( c.x, c.w, c.h, c.y, -1.0f, 1.0f, ortho );
            unsigned int ortho_location = glGetUniformLocation( shader, "ortho" );
            glUniformMatrix4fv( ortho_location, 1, GL_FALSE, ( float * )( ortho ) );
        }

        switch ( layers[ i ].type )
        {
            case ( GFX_RECT ):
            {
                draw_box
                (
                    &layers[ i ].data.rect.rect,
                    &layers[ i ].data.rect.color,
                    &layers[ i ].data.rect.color,
                    &layers[ i ].data.rect.color,
                    &layers[ i ].data.rect.color
                );
            }
            break;
            case ( GFX_TEXT ):
            {
                for ( int c = 0; c < layers[ i ].data.text.number_of_characters; ++c )
                {
                    character( &layers[ i ].data.text.characters[ c ], &layers[ i ].data.text.color );
                }
            }
            break;
            case ( GFX_SPRITE ):
            {
                sprite
                (
                    layers[ i ].data.sprite.texture,
                    layers[ i ].data.sprite.palette,
                    &layers[ i ].data.sprite.src,
                    &layers[ i ].data.sprite.dest,
                    layers[ i ].data.sprite.flip_x,
                    layers[ i ].data.sprite.flip_y,
                    layers[ i ].data.sprite.rotation_x,
                    layers[ i ].data.sprite.rotation_y,
                    layers[ i ].data.sprite.rotation_z
                );
            }
            break;
            case ( GFX_TILEMAP ):
            {
                TilemapGraphics * tg = &layers[ i ].data.tilemap;
                glUseProgram( tilemap_shader );

                Rect src = { 0.0f, 0.0f, ( float )( textures[ tg->tilemap ].width ), ( float )( textures[ tg->tilemap ].height ) };
                Rect dest = { 0.0f, 0.0f, ( float )( textures[ tg->tilemap ].width ) * 16.0f, ( float )( textures[ tg->tilemap ].height ) * 16.0f };

                vertices[ 2 + VERTEX_SIZE * 3 ] = vertices[ 2 + VERTEX_SIZE * 2 ] = 1.0f / ( float )( textures[ tg->tilemap ].width ) * src.x; // Left X
                vertices[ 2 ] = vertices[ 2 + VERTEX_SIZE ] = 1.0f / ( float )( textures[ tg->tilemap ].width ) * ( src.x + src.w );  // Right X
                vertices[ 3 + VERTEX_SIZE * 3 ] = vertices[ 3 ] = 1.0f / ( float )( textures[ tg->tilemap ].height ) * ( src.y + src.h ); // Top Y
                vertices[ 3 + VERTEX_SIZE * 2 ] = vertices[ 3 + VERTEX_SIZE ] = 1.0f / ( float )( textures[ tg->tilemap ].height ) * src.y;  // Bottom Y
                buffer_vertices();
                set_vertices_view( dest.x + ( dest.w / 2.0f ), dest.y + ( dest.h / 2.0f ) );

                mat4 model = BASE_MATRIX;
                vec3 scale = { dest.w, dest.h, 0.0 };
                glm_scale( model, scale );
                unsigned int model_location = glGetUniformLocation(tilemap_shader, "model");
                glUniformMatrix4fv( model_location, 1, GL_FALSE, ( float * )( model ) );

                GLint palette_id_location = glGetUniformLocation( tilemap_shader, "palette_id" );
                glUniform1f( palette_id_location, ( float )( 3 ) );

                GLint map_width_location = glGetUniformLocation( tilemap_shader, "map_width" );
                glUniform1f( map_width_location, ( float )( textures[ tg->tilemap ].width ) );

                GLint map_height_location = glGetUniformLocation( tilemap_shader, "map_height" );
                glUniform1f( map_height_location, ( float )( textures[ tg->tilemap ].height ) );

                GLint tileset_width_location = glGetUniformLocation( tilemap_shader, "tileset_width" );
                glUniform1f( tileset_width_location, ( float )( textures[ tg->texture ].width ) );

                GLint tileset_height_location = glGetUniformLocation( tilemap_shader, "tileset_height" );
                glUniform1f( tileset_height_location, ( float )( textures[ tg->texture ].height ) );

                GLint texture_data_location = glGetUniformLocation(tilemap_shader, "texture_data");
                GLint palette_data_location = glGetUniformLocation(tilemap_shader, "palette_data");
                GLint map_data_location = glGetUniformLocation(tilemap_shader, "map_data");
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_ids[ tg->texture ] );
                glUniform1i(texture_data_location, 0);
                glActiveTexture(GL_TEXTURE1 );
                glBindTexture(GL_TEXTURE_2D, texture_ids[ palette_texture ] );
                glUniform1i(palette_data_location, 1);
                glActiveTexture(GL_TEXTURE2 );
                glBindTexture(GL_TEXTURE_2D, texture_ids[ tg->tilemap ] );
                glUniform1i(map_data_location, 2);
                render_vertices();
            }
            break;
        }
    }
    glfwSwapBuffers( window );
};

void render_clear_textures()
{
    // Clear all textures ’cept palette & charset.
    for ( int i = 0; i < TEXTURE_MAP_CAPACITY; ++i )
    {
        if
        (
            texture_map[ i ].key.string != NULL &&
            strcmp( texture_map[ i ].key.string, "sprites/palette.png" ) != 0 &&
            strcmp( texture_map[ i ].key.string, "text/latin.png" ) != 0
        )
        {
            free( texture_map[ i ].key.string );
            texture_map[ i ].key.hash = 0;
        }
    }
    number_of_textures = 2;
};

unsigned int render_get_texture_id( const char * local )
{
    return render_get_texture_id_generic( local, 1 );
};

unsigned int render_get_texture_id_noindex( const char * local )
{
    return render_get_texture_id_generic( local, 0 );
};

unsigned int render_get_texture_id_generic( const char * local, int indexed )
{
    const render_hash_t needle_hash = hash_string( local );
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
    if ( texture_data == NULL )
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
    for ( int i = 0; i < MAX_STATES; ++i )
    {
        for ( int j = 0; j < NUMBER_OF_LAYERS; ++j )
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
        for ( int li = 0; li < NUMBER_OF_LAYERS; ++li )
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
    for ( int li = 0; li <= NUMBER_OF_LAYERS; ++li )
    {
        number_of_graphics -= graphics_per_layer[ state ][ li ];
        graphics_per_layer[ state ][ li ] = 0;
    }

    // TODO: If it’s possible to clear graphics o’ state that isn’t @ the top,
    // move later states’ graphics down to next blank space.
};

int render_window_should_close()
{
    return glfwWindowShouldClose( window );
};

void * render_get_window()
{
    return ( void * )( window );
};

void render_adjust_camera( Rect * target, float max_w, float max_h )
{
    float x_adjust = 0.0f;
    const float right_edge = WINDOW_WIDTH_PIXELS * 0.75 + camera.x;
    const float left_edge = WINDOW_WIDTH_PIXELS * 0.25 + camera.x;
    const float target_right = RECT_RIGHT( target );
    
    if ( target_right > right_edge )
    {
        x_adjust = target_right - right_edge;
    }
    else if ( target->x < left_edge )
    {
        x_adjust = target->x - left_edge;
    }
    camera.x += x_adjust;
    camera.w += x_adjust;
    if ( camera.x < 0.0f )
    {
        camera.w = WINDOW_WIDTH_PIXELS;
        camera.x = 0.0f;
    }
    else if ( camera.w > max_w )
    {
        camera.x = max_w - WINDOW_WIDTH_PIXELS;
        camera.w = max_w;
    }

    float y_adjust = 0.0f;
    const float bottom_edge = WINDOW_HEIGHT_PIXELS * 0.75 + camera.y;
    const float top_edge = WINDOW_HEIGHT_PIXELS * 0.25 + camera.y;
    const float target_bottom = RECT_BOTTOM( target );
    
    if ( target_bottom > bottom_edge )
    {
        y_adjust = target_bottom - bottom_edge;
    }
    else if ( target->y < top_edge )
    {
        y_adjust = target->y - top_edge;
    }
    camera.y += y_adjust;
    camera.h += y_adjust;
    if ( camera.y < 0.0f )
    {
        camera.h = WINDOW_HEIGHT_PIXELS;
        camera.y = 0.0f;
    }
    else if ( camera.h > max_h )
    {
        camera.y = max_h - WINDOW_HEIGHT_PIXELS;
        camera.h = max_h;
    }
};

void draw_box( const Rect * rect, const Color * top_left_color, const Color * top_right_color, const Color * bottom_left_color, const Color * bottom_right_color )
{
    glUseProgram( rect_shader );
    set_vertices_colors( top_left_color, top_right_color, bottom_left_color, bottom_right_color );
    buffer_vertices();
    set_vertices_view( rect->x + ( rect->w / 2.0f ), rect->y + ( rect->h / 2.0f ) );
    mat4 model = BASE_MATRIX;
    vec3 scale = { rect->w, rect->h, 0.0 };
    glm_scale( model, scale );
    unsigned int model_location = glGetUniformLocation( rect_shader, "model" );
    glUniformMatrix4fv( model_location, 1, GL_FALSE, ( float * )( model ) );
    render_vertices();
};

void framebuffer_size_callback( GLFWwindow * window, int screen_width, int screen_height )
{
    double screen_aspect_ratio = ( double )( canvas_width / canvas_height );
    double monitor_aspect_ratio = ( double )( screen_width ) / ( double )( screen_height );

    magnification = 
        ( int )( floor(
            ( monitor_aspect_ratio > screen_aspect_ratio )
                ? ( double )( screen_height ) / ( double )( canvas_height )
                : ( double )( screen_width ) / ( double )( canvas_width )
        ));
    if ( magnification < 1 )
    {
        magnification = 1;
    }

    GLint magnified_canvas_width = canvas_width * magnification;
    GLint magnified_canvas_height = canvas_height * magnification;
    GLint x = ( int )( floor( ( double )( screen_width - magnified_canvas_width ) / 2.0 ) );
    GLint y = ( int )( floor( ( double )( screen_height - magnified_canvas_height ) / 2.0 ) );
    glViewport( x, y, magnified_canvas_width, magnified_canvas_height );
}

unsigned int render_add_graphic( Graphic gfx, int state, Layer layer )
{

    // Count up graphics to where current graphic should be.
    int i = 0;
    for ( int si = 0; si < state; ++si )
    {
        for ( int li = 0; li < NUMBER_OF_LAYERS; ++li )
        {
            i += graphics_per_layer[ si ][ li ];
        }
    }
    for ( int li = 0; li <= layer; ++li )
    {
        i += graphics_per_layer[ state ][ li ];
    }

    // Move all graphics ’bove where current should go forward 1.
    for ( int gi = number_of_graphics - 1; gi >= i; --gi )
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
    ++graphics_per_layer[ state ][ layer ];
    ++number_of_graphics;
    return map_index;
};

Graphic * render_get_graphic( unsigned int id )
{
    return &layers[ graphics_map[ id ] ];
};

static void sprite( unsigned int texture_id, unsigned int palette_id, const Rect * src, const Rect * dest, int flip_x, int flip_y, float rotation_x, float rotation_y, float rotation_z )
{
    glUseProgram( sprite_shader );

    // Src Coords
    if ( flip_x )
    {
        vertices[ 2 ] = vertices[ 2 + VERTEX_SIZE ] = 1.0f / ( float )( textures[ texture_id ].width ) * src->x; // Left X
        vertices[ 2 + VERTEX_SIZE * 3 ] = vertices[ 2 + VERTEX_SIZE * 2 ] = 1.0f / ( float )( textures[ texture_id ].width ) * ( src->x + src->w );  // Right X
    }
    else
    {
        vertices[ 2 + VERTEX_SIZE * 3 ] = vertices[ 2 + VERTEX_SIZE * 2 ] = 1.0f / ( float )( textures[ texture_id ].width ) * src->x; // Left X
        vertices[ 2 ] = vertices[ 2 + VERTEX_SIZE ] = 1.0f / ( float )( textures[ texture_id ].width ) * ( src->x + src->w );  // Right X
    }

    if ( flip_y )
    {
        vertices[ 3 + VERTEX_SIZE * 2 ] = vertices[ 3 + VERTEX_SIZE ] = 1.0f / ( float )( textures[ texture_id ].height ) * ( src->y + src->h ); // Top Y
        vertices[ 3 + VERTEX_SIZE * 3 ] = vertices[ 3 ] = 1.0f / ( float )( textures[ texture_id ].height ) * src->y;  // Bottom Y
    }
    else
    {
        vertices[ 3 + VERTEX_SIZE * 3 ] = vertices[ 3 ] = 1.0f / ( float )( textures[ texture_id ].height ) * ( src->y + src->h ); // Top Y
        vertices[ 3 + VERTEX_SIZE * 2 ] = vertices[ 3 + VERTEX_SIZE ] = 1.0f / ( float )( textures[ texture_id ].height ) * src->y;  // Bottom Y
    }

    buffer_vertices();
    set_vertices_view( dest->x + ( dest->w / 2.0f ), dest->y + ( dest->h / 2.0f ) );

    mat4 model = BASE_MATRIX;
    vec3 scale = { dest->w, dest->h, 0.0 };
    glm_scale( model, scale );
    vec3 xrot = { 0.0, 1.0, 0.0 };
    glm_rotate( model, DEGREES_TO_RADIANS( rotation_x ), xrot );
    vec3 yrot = { 0.0, 0.0, 1.0 };
    glm_rotate( model, DEGREES_TO_RADIANS( rotation_y ), yrot );
    vec3 zrot = { 1.0, 0.0, 0.0 };
    glm_rotate( model, DEGREES_TO_RADIANS( rotation_z ), zrot );
    unsigned int model_location = glGetUniformLocation(sprite_shader, "model");
    glUniformMatrix4fv( model_location, 1, GL_FALSE, ( float * )( model ) );

    GLint palette_id_location = glGetUniformLocation( sprite_shader, "palette_id" );
    glUniform1f( palette_id_location, ( float )( palette_id ) );

    GLint texture_data_location = glGetUniformLocation(sprite_shader, "texture_data");
    GLint palette_data_location = glGetUniformLocation(sprite_shader, "palette_data");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_ids[ texture_id ] );
    glUniform1i(texture_data_location, 0);
    glActiveTexture(GL_TEXTURE1 );
    glBindTexture(GL_TEXTURE_2D, texture_ids[ palette_texture ] );
    glUniform1i(palette_data_location, 1);
    render_vertices();
};

static void character( const Character * character, const Color * color )
{
    glUseProgram(text_shader);

    // Src Coords
        vertices[ 2 + VERTEX_SIZE * 3 ] = vertices[ 2 + VERTEX_SIZE * 2 ] = 1.0f / ( float )( textures[ text_texture ].width ) * character->src_x; // Left X
        vertices[ 2 ] = vertices[ 2 + VERTEX_SIZE ] = 1.0f / ( float )( textures[ text_texture ].width ) * ( character->src_x + character->w );  // Right X
        vertices[ 3 + VERTEX_SIZE * 3 ] = vertices[ 3 ] = 1.0f / ( float )( textures[ text_texture ].height ) * ( character->src_y + character->h ); // Top Y
        vertices[ 3 + VERTEX_SIZE * 2 ] = vertices[ 3 + VERTEX_SIZE ] = 1.0f / ( float )( textures[ text_texture ].height ) * character->src_y;  // Bottom Y

    buffer_vertices();
    set_vertices_view( character->dest_x + ( character->w / 2.0f ), character->dest_y + ( character->h / 2.0f ) );

    mat4 model = BASE_MATRIX;
    vec3 scale = { character->w, character->h, 0.0 };
    glm_scale( model, scale );
    unsigned int model_location = glGetUniformLocation(text_shader, "model");
    glUniformMatrix4fv( model_location, 1, GL_FALSE, (float*)(model));

    unsigned int color_location = glGetUniformLocation(text_shader, "color");
    glUniform4f( color_location, color->r, color->g, color->b, color->a );

    GLint texture_data_location = glGetUniformLocation(text_shader, "texture_data");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_ids[ text_texture ] );
    glUniform1i(texture_data_location, 0);
    render_vertices();
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
        log_error( "ERROR: Failed to create shader program\n" );
        log_error( infoLog );
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
    glShaderSource( shader, 1, ( const GLchar * const* )( &string ), NULL );
    free( string );
    glCompileShader( shader );
    glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
    if( !success )
    {
        glGetShaderInfoLog( shader, 512, NULL, infoLog );
        log_error( "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" );
        log_error( infoLog );
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

static TextureMapEntry * hash_find_entry( const char * needle_string, render_hash_t needle_hash )
{
    while ( 1 )
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

static void set_vertices_colors( const Color * top_left_color, const Color * top_right_color, const Color * bottom_left_color, const Color * bottom_right_color )
{
    vertices[ 4 ] = bottom_right_color->r / 255.0f;
    vertices[ 5 ] = bottom_right_color->g / 255.0f;
    vertices[ 6 ] = bottom_right_color->b / 255.0f;
    vertices[ 7 ] = bottom_right_color->a / 255.0f;

    vertices[ 4 + VERTEX_SIZE ] = top_right_color->r / 255.0f;
    vertices[ 5 + VERTEX_SIZE ] = top_right_color->g / 255.0f;
    vertices[ 6 + VERTEX_SIZE ] = top_right_color->b / 255.0f;
    vertices[ 7 + VERTEX_SIZE ] = top_right_color->a / 255.0f;

    vertices[ 4 + VERTEX_SIZE * 2 ] = top_left_color->r / 255.0f;
    vertices[ 5 + VERTEX_SIZE * 2 ] = top_left_color->g / 255.0f;
    vertices[ 6 + VERTEX_SIZE * 2 ] = top_left_color->b / 255.0f;
    vertices[ 7 + VERTEX_SIZE * 2 ] = top_left_color->a / 255.0f;

    vertices[ 4 + VERTEX_SIZE * 3 ] = bottom_left_color->r / 255.0f;
    vertices[ 5 + VERTEX_SIZE * 3 ] = bottom_left_color->g / 255.0f;
    vertices[ 6 + VERTEX_SIZE * 3 ] = bottom_left_color->b / 255.0f;
    vertices[ 7 + VERTEX_SIZE * 3 ] = bottom_left_color->a / 255.0f;
};

static void set_vertices_view( float x, float y )
{
    mat4 view = BASE_MATRIX;
    vec3 trans = { x, y, 0.0f };
    glm_translate( view, trans );
    unsigned int view_location = glGetUniformLocation( rect_shader, "view" );
    glUniformMatrix4fv( view_location, 1, GL_FALSE, ( float * )( view ) );
};

static void render_vertices()
{
    glBindVertexArray( VAO );
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
    glBindVertexArray( 0 );
};

unsigned int render_add_tilemap( const char * tileset, const int * tiles, int w, int h, unsigned int pal, int state_number, Layer layer )
{
    Graphic gfx;
    gfx.type = GFX_TILEMAP;
    gfx.data.tilemap.palette = pal;

    char * tileset_gfx = filename_local_tileset( tileset );
    gfx.data.tilemap.texture = render_get_texture_id( tileset_gfx );
    free( tileset_gfx );

    textures[ number_of_textures ].width = w;
    textures[ number_of_textures ].height = h;
    unsigned char * texture_data = calloc( w * h * 4, sizeof( unsigned char ) );
    int i4 = 0;
    for ( int i = 0; i < w * h; ++i )
    {
        int v = tiles[ i ] - 4097;
        unsigned char x = ( unsigned char )( v % 256 );
        unsigned char y = ( unsigned char )( floor( v / 256 ) );
        unsigned char z = 0;
        if ( v < 0 )
        {
            x = 255;
            y = 255;
            z = 255;
        }
        printf( "( %d, %d, %d ),", x, y, z );
        texture_data[ i4 ] = x;
        texture_data[ i4 + 1 ] = y;
        texture_data[ i4 + 2 ] = z;
        texture_data[ i4 + 3 ] = 255;
        i4 += 4;
    }
    printf( "\n");
    if ( texture_data == NULL )
    {
        log_error( "Couldn’t generate tilemap." );
    }
    glBindTexture(GL_TEXTURE_2D, texture_ids[ number_of_textures ] );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textures[ number_of_textures ].width, textures[ number_of_textures ].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    gfx.data.tilemap.tilemap = number_of_textures++;
    return render_add_graphic( gfx, state_number, layer );
};
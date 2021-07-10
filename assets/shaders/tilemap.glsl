#version 330 core
out vec4 final_color;

in vec2 texture_coords;

uniform sampler2D texture_data;
uniform sampler2D palette_data;
uniform sampler2D map_data;
uniform float palette_id;
uniform float map_width;
uniform float map_height;
uniform float tileset_width;
uniform float tileset_height;
  
void main()
{
    vec4 tile = texture( map_data, texture_coords );
    float xrel = mod( texture_coords.x * 256.0, ( 256.0 / map_width ) ) / ( 4096.0 / map_width );
    float yrel = mod( texture_coords.y * 256.0, ( 256.0 / map_height ) ) / ( 4096.0 / map_height );
    float xoffset = tile.x * 255.0 * ( 16 / tileset_width );
    float yoffset = tile.y * 255.0 * ( 16 / tileset_height );
    vec4 index = texture( texture_data, vec2( xoffset + ( xrel / ( tileset_width / 256.0 ) ), yoffset + ( yrel / ( tileset_height / 256.0 ) ) ) );
    final_color = ( tile.a < 1.0 ) ? texture( palette_data, vec2( index.r / 16.0, palette_id ) ) : vec4( 0.0, 0.0, 0.0, 0.0 );
}
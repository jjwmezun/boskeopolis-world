#version 330 core
out vec4 final_color;

in vec2 texture_coords;

uniform sampler2D texture_data;
uniform sampler2D palette_data;
uniform float palette_id;
uniform float opacity;
  
void main()
{
    vec4 index = texture( texture_data, texture_coords );
    float palette = palette_id / 256.0;
    final_color = texture( palette_data, vec2( index.r / 16.0, palette ) );
    final_color.a *= opacity;
}
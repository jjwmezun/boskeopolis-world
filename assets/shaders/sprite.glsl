#version 330 core
out vec4 final_color;

in vec2 texture_coords;

uniform sampler2D texture_data;
uniform sampler2D palette_data;
uniform float palette_id;
  
void main()
{
    vec4 index = texture( texture_data, texture_coords );
    final_color = texture( palette_data, vec2( index.r / 16.0, palette_id ) );
}
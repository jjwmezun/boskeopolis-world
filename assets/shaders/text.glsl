#version 330 core
out vec4 final_color;

in vec2 texture_coords;

uniform sampler2D texture_data;
uniform vec4 color;
  
void main()
{
    final_color = vec4( color.rgb, color.a * texture( texture_data, texture_coords ).a );
}
#version 330 core
out vec4 final_color;

in vec2 texture_coords;

uniform sampler2D texture_data;
  
void main()
{
    final_color = texture( texture_data, texture_coords );
}
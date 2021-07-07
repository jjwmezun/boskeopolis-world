#version 330 core
out vec4 final_color;

in vec4 out_color;
in vec2 out_position;

uniform vec4 color;
  
void main()
{
    final_color = out_color;
}
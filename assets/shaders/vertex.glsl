#version 330 core
layout ( location = 0 ) in vec2 in_position;
layout ( location = 1 ) in vec2 in_texture_coords;
layout ( location = 2 ) in vec4 in_color;

out vec2 texture_coords;
out vec4 out_color;
out vec2 out_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 ortho;

void main()
{
   out_position = in_position;
   gl_Position = ortho * view * model * vec4( in_position, 0.0, 1.0 );
   texture_coords = in_texture_coords;
   out_color = in_color;
}
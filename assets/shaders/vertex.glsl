#version 330 core
layout ( location = 0 ) in vec2 in_position;
layout ( location = 1 ) in vec2 in_texture_coords;

out vec2 texture_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 ortho;

void main()
{
   gl_Position = ortho * view * model * vec4( in_position, 0.0, 1.0 );
   texture_coords = in_texture_coords;
}
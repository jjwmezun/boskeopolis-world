#version 330 core
out vec4 final_color;

in vec4 out_color;
in vec2 out_position;

uniform vec4 color;
  
void main()
{
    //final_color = ( ( floor( ( out_color * 256.0 ) / 8.0 ) * 8.0 ) / 256.0 ) - ( ( ( ( out_position.y * 256.0 / 1.0 ) - floor( out_position.y * 256.0 / 1.0 ) ) / 256.0 ) * 8.0 ) + ( ( ( ( out_position.x * 256.0 / 1.0 ) - floor( out_position.x * 256.0 / 1.0 ) ) / 256.0 ) * 8.0 );
    final_color = out_color;
}
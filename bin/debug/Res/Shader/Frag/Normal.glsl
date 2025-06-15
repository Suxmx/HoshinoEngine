#version 330 core
			
layout(location = 0) out vec4 color;

in vec3 v_Position;
uniform vec3 u_AlbedoColor;


void main()
{
    color = vec4(u_AlbedoColor, 1.0);
}
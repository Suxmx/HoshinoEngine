#version 330 core
			
layout(location = 0) out vec4 color;

uniform sampler2D ourTexture;

in vec3 v_Position;
in vec2 v_TexCoord;

void main()
{
    color =texture(ourTexture, v_TexCoord) * vec4(1, 1,1,1);
}
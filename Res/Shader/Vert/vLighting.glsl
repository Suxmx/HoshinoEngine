#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Binormal;
layout(location = 4) in vec2 a_TexCoord;
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 TexCoords;

void main()
{
    TexCoords = (u_ViewProjection * u_Transform * vec4(a_Position, 1.0)).xy/2+vec2(0.5, 0.5);
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}
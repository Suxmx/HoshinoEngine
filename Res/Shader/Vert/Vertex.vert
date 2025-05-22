#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Binormal;
layout(location = 4) in vec2 a_TexCoord;
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Position;
out vec2 v_TexCoord;

void main()
{
    v_Position = a_Position;
    v_TexCoord = vec2(a_TexCoord.x, a_TexCoord.y);
    vec3 holder = a_Normal + a_Tangent + a_Binormal -a_Normal - a_Tangent - a_Binormal + vec3(a_TexCoord.x, a_TexCoord.y, 0.0)  - vec3(a_TexCoord.x, a_TexCoord.y, 0.0);
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0)+ vec4(holder, 0.0) - vec4(holder, 0.0);	
} 
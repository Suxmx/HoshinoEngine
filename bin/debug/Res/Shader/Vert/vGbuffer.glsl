#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Binormal;
layout(location = 4) in vec2 a_TexCoord;
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

void main()
{
    vec4 worldPos = u_Transform * vec4(a_Position, 1.0);
    FragPos = worldPos.xyz; 
    TexCoords = a_TexCoord;
    
    // Transform normal from object space to world space
    mat3 normalMatrix = transpose(inverse(mat3(u_Transform)));
    Normal = normalize(normalMatrix * a_Normal);

    gl_Position = u_ViewProjection * worldPos;
}
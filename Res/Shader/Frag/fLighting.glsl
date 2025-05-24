#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

// struct Light {
//     vec3 Position;
//     vec3 Color;
    
//     float Linear;
//     float Quadratic;
// };
// const int NR_LIGHTS = 32;
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{             
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    vec3 Normal = texture(gNormal, TexCoords).rgb;

    // then calculate lighting as usual
    // vec3 lighting  = Diffuse * 0.1; // hard-coded ambient component
    // vec3 viewDir  = normalize(viewPos - FragPos);
    // for(int i = 0; i < NR_LIGHTS; ++i)
    // {
    //     // diffuse
    //     vec3 lightDir = normalize(lights[i].Position - FragPos);
    //     vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lights[i].Color;
    //     // specular
    //     vec3 halfwayDir = normalize(lightDir + viewDir);  
    //     float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
    //     vec3 specular = lights[i].Color * spec * Specular;
    //     // attenuation
    //     float distance = length(lights[i].Position - FragPos);
    //     float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
    //     diffuse *= attenuation;
    //     specular *= attenuation;
    //     lighting += diffuse + specular;        
    // }
    // 防止裁剪
    FragColor = vec4(vec3(1)+FragPos+Normal-FragPos-Normal+Diffuse, 1.0);
}
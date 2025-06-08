// 基本片段着色器
// 用于简单3D渲染的光照和纹理

// 常量缓冲区定义
cbuffer MaterialBuffer : register(b1)
{
    float4 u_DiffuseColor;        // 漫反射颜色
    float3 u_LightDirection;      // 光照方向
    float u_AmbientStrength;      // 环境光强度
    float3 u_LightColor;          // 光照颜色
    float u_SpecularStrength;     // 高光强度
    float3 u_ViewPosition;        // 相机位置
    float u_Shininess;            // 光泽度
}

// 纹理和采样器
Texture2D u_DiffuseTexture : register(t0);
SamplerState u_Sampler : register(s0);

// 顶点着色器传入的数据
struct PS_INPUT
{
    float4 Position : SV_POSITION;   // 屏幕空间位置
    float3 Normal : NORMAL;          // 法线
    float2 TexCoord : TEXCOORD0;     // 纹理坐标
    float3 WorldPos : TEXCOORD1;     // 世界空间位置
};

// 片段着色器主函数
float4 main(PS_INPUT input) : SV_TARGET
{
    // 归一化法线
    float3 normal = normalize(input.Normal);
    
    // 光照方向
    float3 lightDir = normalize(-u_LightDirection);
    
    // 环境光
    float3 ambient = u_AmbientStrength * u_LightColor;
    
    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    float3 diffuse = diff * u_LightColor;
    
    // 镜面反射
    float3 viewDir = normalize(u_ViewPosition - input.WorldPos);
    float3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);
    float3 specular = u_SpecularStrength * spec * u_LightColor;
    
    // 采样纹理
    float4 texColor = u_DiffuseTexture.Sample(u_Sampler, input.TexCoord);
    
    // 合并光照和纹理
    float3 result = (ambient + diffuse + specular) * texColor.rgb * u_DiffuseColor.rgb;
    
    return float4(result, texColor.a * u_DiffuseColor.a);
}
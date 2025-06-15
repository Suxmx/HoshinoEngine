// 基本顶点着色器
// 用于简单3D渲染的顶点变换

// 常量缓冲区定义
cbuffer TransformBuffer : register(b0)
{
    matrix u_Model;        // 模型矩阵
    matrix u_View;         // 视图矩阵
    matrix u_Projection;   // 投影矩阵
}

// 顶点着色器输入结构
struct VS_INPUT
{
    float3 Position : POSITION;     // 顶点位置
    float3 Normal : NORMAL;         // 顶点法线
    float2 TexCoord : TEXCOORD0;    // 纹理坐标
};

// 顶点着色器输出结构
struct VS_OUTPUT
{
    float4 Position : SV_POSITION;   // 变换后的位置
    float3 Normal : NORMAL;          // 变换后的法线
    float2 TexCoord : TEXCOORD0;     // 纹理坐标
    float3 WorldPos : TEXCOORD1;     // 世界空间位置
};

// 顶点着色器主函数
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    // 计算世界空间位置
    float4 worldPosition = mul(float4(input.Position, 1.0f), u_Model);
    
    // 计算裁剪空间位置
    output.Position = mul(mul(worldPosition, u_View), u_Projection);
    
    // 计算世界空间法线
    output.Normal = mul(input.Normal, (float3x3)u_Model);
    
    // 传递纹理坐标
    output.TexCoord = input.TexCoord;
    
    // 世界空间位置
    output.WorldPos = worldPosition.xyz;
    
    return output;
}
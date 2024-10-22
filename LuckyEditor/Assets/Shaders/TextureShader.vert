#version 450 core

layout(location = 0) in vec3 a_Position;    // 位置
layout(location = 1) in vec4 a_Color;       // 颜色
layout(location = 2) in vec2 a_TexCoord;    // 纹理坐标
layout(location = 3) in float a_TexIndex;   // 纹理索引
layout(location = 4) in int a_ObjectID;     // 物体 ID

// 相机 Uniform 缓冲区
layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjectionMatrix;
};

// 顶点着色器输出数据
struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
    float TexIndex;
};

layout(location = 0) out VertexOutput v_Output;
layout(location = 3) out flat int v_ObjectID;

void main()
{
    v_Output.Color = a_Color;
    v_Output.TexCoord = a_TexCoord;
    v_Output.TexIndex = a_TexIndex;
    
    v_ObjectID = a_ObjectID;

    gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}
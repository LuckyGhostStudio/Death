#version 450 core

layout(location = 0) out vec4 o_Color;      // 颜色缓冲区 0 输出颜色
layout(location = 1) out int o_ObjectID;    // 颜色缓冲区 1 输出颜色（物体 ID）

// 顶点着色器输出数据
struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
    float TexIndex;
};

layout(location = 0) in VertexOutput v_Input;
layout(location = 3) in flat int v_ObjectID;

layout(binding = 0) uniform sampler2D u_Textures[32];   // 纹理 0 - 31

void main()
{
    o_Color = texture(u_Textures[int(v_Input.TexIndex)], v_Input.TexCoord) * v_Input.Color;

    o_ObjectID = v_ObjectID;
}
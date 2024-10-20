#version 450

layout(location = 0) out vec4 o_Color;      // ��ɫ������ 0 �����ɫ
layout(location = 1) out int o_ObjectID;    // ��ɫ������ 1 �����ɫ������ ID��

in vec4 v_Color;
in vec2 v_TexCoord;
in flat float v_TexIndex;
in flat int v_ObjectID;

uniform sampler2D u_Textures[32];   // ���� 0 - 31

void main()
{
    o_Color = texture(u_Textures[int(v_TexIndex)], v_TexCoord) * v_Color;
    o_ObjectID = v_ObjectID;
}
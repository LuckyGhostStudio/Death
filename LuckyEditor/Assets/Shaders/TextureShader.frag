#version 330 core

layout(location = 0) out vec4 o_Color;  // ��ɫ������ 0 �����ɫ
layout(location = 1) out int o_FragID;  // ��ɫ������ 1 �����ɫ������ id��

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[32];   // ���� 0 - 31

void main()
{
    o_Color = texture(u_Textures[int(v_TexIndex)], v_TexCoord) * v_Color;
    o_FragID = 50;  // Test
}
#version 330 core

layout(location = 0) in vec3 a_Position;	// λ��
layout(location = 1) in vec4 a_Color;		// ��ɫ
layout(location = 2) in vec2 a_TexCoord;	// ��������
layout(location = 3) in float a_TexIndex;	// ��������

uniform mat4 u_ViewProjectionMatrix;	// VP ����

out vec4 v_Color;
out vec2 v_TexCoord;	
out float v_TexIndex;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;

	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}
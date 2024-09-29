#version 330 core

layout(location = 0) out vec4 o_Color;  // Êä³öÑÕÉ«

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[32];   // ÎÆÀí 0 - 31

void main()
{
    o_Color = texture(u_Textures[int(v_TexIndex)], v_TexCoord) * v_Color;
}
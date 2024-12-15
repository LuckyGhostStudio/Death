#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in int a_ObjectID;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjection;
};

struct VertexOutput
{
    vec4 Color;
};

layout (location = 0) out VertexOutput Output;
layout (location = 1) out flat int v_ObjectID;

void main()
{
    Output.Color = a_Color;
    v_ObjectID = a_ObjectID;

    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}
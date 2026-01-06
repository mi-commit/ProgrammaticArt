#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 Color;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 VertexColor;
out vec2 TexCoord;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0f);
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
	VertexColor = Color;
	TexCoord = aTexCoord;
}
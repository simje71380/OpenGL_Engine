#version 330 core

layout(location = 0) out vec4 color;

in vec4 fColor;
in vec3 fNormals;

uniform vec3 Ka;

void main()
{
	color = vec4(Ka, 1.0f);
};
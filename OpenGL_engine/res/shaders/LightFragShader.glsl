#version 330 core

layout(location = 0) out vec4 color;

in vec3 fNormals;
in vec3 FragPos;

uniform vec3 lightColor;

void main()
{
	color = vec4(lightColor, 1.0f);
};
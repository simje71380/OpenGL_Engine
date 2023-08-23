#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;
in vec4 fColor;

void main()
{
	color = vec4(u_Color.x, fColor);
};
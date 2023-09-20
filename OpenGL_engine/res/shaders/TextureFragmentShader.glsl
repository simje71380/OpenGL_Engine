#version 330 core

layout(location = 0) out vec4 color;

in vec4 fColor;

in vec2 texCoord;

uniform sampler2D tex0;

void main()
{
	color = texture(tex0, texCoord);
};
#version 330 core

layout(location = 0) out vec4 color;

in vec4 fColor;
in vec2 TexCoord;
in vec3 fKa;

uniform sampler2D Texture;

void main()
{
	color = texture(Texture, TexCoord);
};
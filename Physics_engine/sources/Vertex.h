#pragma once
#include "glm/glm.hpp"

class RGBA {
private:

public:
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 1.0f;

	RGBA(){}
	RGBA(float _r, float _g, float _b, float _a) 
		: r(_r), g(_g), b(_b), a(_a){}
	~RGBA(){}
};


class Vertex {
private:

public:
	Vertex(float x, float y, float z, float r, float g, float b, float a)
		: position(x,y,z,1.0f), color(r,g,b,a){};
	~Vertex() {};

	Vertex(glm::vec4 pos, float r, float g, float b, float a)
		: position(pos), color(r, g, b, a) {};

	Vertex(glm::vec3 pos, float r, float g, float b, float a)
		: position(pos, 1.0f), color(r, g, b, a) {};

	Vertex(glm::vec4 pos, float r, float g, float b)
		: position(pos), color(r, g, b, 1.0f) {};

	Vertex(glm::vec3 pos, float r, float g, float b)
		: position(pos, 1.0f), color(r, g, b, 1.0f) {};

	Vertex(glm::vec4 pos, RGBA _color)
		: position(pos), color(_color) {};

	glm::vec4 position;
	RGBA color;
};

#pragma once
#include "../OpenGL_engine/libs/glm/glm/glm.hpp"

//Vertex : Vec4 pos, Vec2 UV, Vec3, normal
class Vertex {
private:

public:
	glm::vec4 position;
	glm::vec2 UV;
	glm::vec3 normal;

	Vertex(glm::vec4 pos, glm::vec2 uv, glm::vec3 normals)
		: position(pos), UV(uv), normal(normals) {};

	Vertex(glm::vec3 pos, glm::vec2 uv, glm::vec3 normals)
		: position(pos, 1.0f), UV(uv), normal(normals) {};

	Vertex()
		: position(0.0f, 0.0f, 0.0f, 1.0f), UV(0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f) {};

	void Set(Vertex v) {
		position = v.position;
		UV = v.UV;
		normal = v.normal;
	}

	~Vertex() {};
};

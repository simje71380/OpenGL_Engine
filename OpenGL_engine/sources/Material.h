#pragma once
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"


class Material {
private:

public:
	Material(Shader* shader);
	~Material() {};

	void Bind();
	void Unbind();

	GLuint m_MatrixID;
	Shader* m_Shader = NULL;
	Texture* m_Texture = NULL;
	char name[128];
	float Ns;
	GLuint m_NsID;
	glm::vec3 Ka;
	GLuint m_KaID;
	glm::vec3 Kd;
	GLuint m_KdID;
	glm::vec3 Ks;
	GLuint m_KsID;
	glm::vec3 Ke;
	GLuint m_KeID;
	float Ni;
	GLuint m_NiID;
	float d;
	GLuint m_dID;
	float illum;
	GLuint m_illumID;
};
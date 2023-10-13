#pragma once
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"


class Material {
private:
	bool m_IsLight = false;
public:
	Material(Shader* shader);
	Material(Shader* shader, bool is_light);
	~Material() {};

	void Bind();
	void Unbind();

	GLuint m_ViewMatrixID;
	GLuint m_ProjMatrixID;
	GLuint m_ModelMatrixID;

	GLuint m_ViewPos;

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
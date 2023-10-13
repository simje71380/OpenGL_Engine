#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "glm/glm.hpp"
#include "Vertex.h"
#include <iostream>
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "Material.h"
#include "common/utils.h"
#include "MaterialsManager.h"

struct index_mat{
	unsigned int m_Indices_Count;
	unsigned int* m_Indices;
	IndexBuffer* m_ib;
	Material* m_mat;
};


class Object3D {
private:
	unsigned int m_Vertices_Count;
	Vertex* m_Vertices;


	VertexBuffer* m_vb;
	VertexBufferLayout* m_layout;

	std::vector<Material*> LoadMaterial(const char* file_path, MaterialsManager* MM);

	Shader* m_ShaderNonTextured;
	Shader* m_ShaderTextured;

	std::vector<Material*> materials;

public:
	Object3D(unsigned int vertices_count, Vertex* vertices, unsigned int* indices, unsigned int indices_count, Material *material);
	~Object3D();
	//constructor from obj file
	Object3D(const char* file_path, MaterialsManager* MM);
	Object3D(const char* file_path, bool is_light);

	// Model matrix : an identity matrix (model will be at the origin) for most cases
	glm::mat4 m_Model = glm::mat4(1.0f);
	VertexArray* m_va;
	std::vector<index_mat> m_MaterialWrapper;
	glm::vec3 m_Origin = glm::vec3(0.0f, 0.0f, 0.0f);

	void Rotate(float theta_x, float theta_y, float theta_z);
	void Translate(float x, float y, float z);
	void Scale(float x, float y, float z);
	void Scale(float coef);
};
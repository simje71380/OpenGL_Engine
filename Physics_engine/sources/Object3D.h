#pragma once

#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "common/shader.hpp"
#include "glm/glm.hpp"
#include "Vertex.h"
#include <iostream>
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"


class Object3D {
private:
	unsigned int m_Vertices_Count;
	unsigned int m_Indices_Count;
	Vertex* m_Vertices;
	unsigned int* m_Indices;

	VertexArray* m_va;
	VertexBuffer* m_vb;
	IndexBuffer* m_ib;
	VertexBufferLayout* m_layout;
	unsigned int m_shader;
	GLuint m_MatrixID;

	// Model matrix : an identity matrix (model will be at the origin) for most cases
	glm::mat4 m_Model = glm::mat4(1.0f);

public:
	Object3D(unsigned int vertices_count, Vertex* vertices, unsigned int* indices, unsigned int indices_count);
	~Object3D();

	void Rotate(float theta_x, float theta_y, float theta_z);
	void Translate(float x, float y, float z);
	void Scale(float x, float y, float z);

	void Draw(Camera* cam);
};
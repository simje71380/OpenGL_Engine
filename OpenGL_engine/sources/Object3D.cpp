#include "Object3D.h"

Object3D::Object3D(unsigned int vertices_count, Vertex *vertices, unsigned int *indices, unsigned int indices_count)
	: m_Vertices_Count(vertices_count), m_Indices_Count(indices_count), m_Vertices(vertices), m_Indices(indices){


	//setting up the vertex array object
	m_va = new VertexArray;

	//setting up vertex buffer
	m_vb = new VertexBuffer(m_Vertices, m_Vertices_Count * sizeof(Vertex));

	//setting up the VB layout
	m_layout = new VertexBufferLayout;
	m_layout->Push<float>(4);
	m_layout->Push<float>(4);
	m_va->AddBuffer(*m_vb, *m_layout);

	//setting up index buffer
	m_ib = new IndexBuffer(indices, indices_count);

	//shading
	m_shader = new Shader("res/shaders/3DVertexShader.glsl", "res/shaders/3DFragShader.glsl");
	m_shader->Bind(); //set drawing with this shader (vertex + frag)

	// Get a handle for our "MVP" uniform
	m_MatrixID = m_shader->GetUniformLocation("MVP");
}

Object3D::~Object3D()
{
	delete[] m_Vertices;
	delete[] m_Indices;
	delete m_va;
	delete m_vb;
	delete m_ib;
	delete m_layout;
	delete m_shader;
}

void Object3D::Rotate(float theta_x, float theta_y, float theta_z)
{
	//proceed the rotation to the vertex
	glm::mat4 RMatrix_x = glm::rotate(theta_x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 RMatrix_y = glm::rotate(theta_y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 RMatrix_z = glm::rotate(theta_z, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 RMatrix = RMatrix_x * RMatrix_y * RMatrix_z;

	for (unsigned int i = 0; i < m_Vertices_Count; i++) {
		Vertex v(m_Vertices[i]);
		glm::vec4 myVector(v.position);
		m_Vertices[i].position = RMatrix * myVector;
	}

	//update the vertex buffer
	m_vb->Update(m_Vertices, m_Vertices_Count * sizeof(Vertex));
	m_va->AddBuffer(*m_vb, *m_layout);
}

void Object3D::Translate(float x, float y, float z)
{
	//proceed the translation to the vertex
	glm::mat4 TMatrix = glm::translate(glm::mat4(1), glm::vec3(x, y, z));
	for (unsigned int i = 0; i < m_Vertices_Count; i++) {
		Vertex v(m_Vertices[i]);
		glm::vec4 myVector(v.position);
		m_Vertices[i].position = TMatrix * myVector;
	}

	//update the vertex buffer
	m_vb->Update(m_Vertices, m_Vertices_Count * sizeof(Vertex));
	m_va->AddBuffer(*m_vb, *m_layout);
}

void Object3D::Scale(float x, float y, float z)
{
	//proceed the scaling to the vertex
	glm::mat4 SMatrix = glm::scale(glm::vec3(x, y, z));
	for (unsigned int i = 0; i < m_Vertices_Count; i++) {
		Vertex v(m_Vertices[i]);
		glm::vec4 myVector(v.position);
		m_Vertices[i].position = SMatrix * myVector;
	}

	//update the vertex buffer
	m_vb->Update(m_Vertices, m_Vertices_Count * sizeof(Vertex));
	m_va->AddBuffer(*m_vb, *m_layout);
}

void Object3D::Draw(Camera* cam)
{
	m_shader->Bind(); //set drawing with this shader (vertex + frag)
	m_va->Bind();
	m_ib->Bind();

	//getting the MVP matrix
	glm::mat4 mvp = cam->Get_MVP(m_Model);

	// Send our transformation to the currently bound shader, in the "MVP" uniform
	// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	GLCall(glUniformMatrix4fv(m_MatrixID, 1, GL_FALSE, &mvp[0][0]));

	GLCall(glDrawElements(GL_TRIANGLES, m_Indices_Count, GL_UNSIGNED_INT, nullptr));
}


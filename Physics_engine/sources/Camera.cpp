#include "Camera.h"

void Camera::Resize(int width, int height)
{
	m_Width = (float)width;
	m_Height = (float)height;
}

glm::mat4 Camera::Get_MVP(glm::mat4 model)
{
	// Projection matrix : Field of View, aspect ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(m_FOV), m_Width / m_Height, m_Range_Close, m_Range_Far);

	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		m_Position, // Camera position in World Space
		m_Looking_Point, // and looking position
		m_Head_Is_Up ? glm::vec3(0, 1, 0) : glm::vec3(0, -1, 0) // Head is up (set to 0,-1,0 to look upside-down)
	);
	return Projection * View * model;
}

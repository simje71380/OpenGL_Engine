#pragma once
#include "glm/glm.hpp"
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
	glm::vec3 m_Position = glm::vec3(2.0f, 1.0f, 1.0f);
	glm::vec3 m_Looking_Point = glm::vec3(0.0f, 0.0f, 0.0f);
	bool m_Head_Is_Up = true;
	float m_FOV = 90;

	//display range : 0.1 unit <-> 100 units
	float m_Range_Close = 0.1f;
	float m_Range_Far = 100.0f;
	float m_Width;
	float m_Height;

public:
	Camera(float width, float height)
		: m_Width(width), m_Height(height){};
	~Camera() {};
	
	void Resize(int width, int height);

	glm::mat4 Get_MVP(glm::mat4 model);
};
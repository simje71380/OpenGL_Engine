#pragma once
#include "glm/glm.hpp"
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include<glm/gtx/vector_angle.hpp>
#include <GLFW/glfw3.h>


class Camera {
private:
	glm::vec3 m_Orientation = glm::vec3(0.0f, 0.0f, -1.0f); //looking to Z axis

	bool m_Head_Is_Up = true;
	float m_FOV = 90;

	//display range : 0.1 unit <-> 100 units
	float m_Range_Close = 0.1f;
	float m_Range_Far = 100.0f;
	float m_Width;
	float m_Height;

	// Prevents the camera from jumping around when first clicking left click
	bool m_firstClick = true;

	// Adjust the speed of the camera and it's sensitivity when looking around
	float m_speed = 10.0f;
	float m_sensitivity = 100.0f;



public:
	Camera(float width, float height, glm::vec3 position)
		: m_Width(width), m_Height(height), m_Position(position) {};
	~Camera() {};
	
	void Resize(int width, int height);

	glm::mat4 Get_MVP(glm::mat4 model);
	glm::mat4 Get_View();
	glm::mat4 Get_Proj();

	glm::vec3 m_Position = glm::vec3(1.0f, 1.0f, 1.0f);

	// Handles camera inputs
	void Inputs(GLFWwindow* window, double deltaTime);
};
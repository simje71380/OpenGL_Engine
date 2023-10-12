#pragma once

#include "glm/glm.hpp"

class PointLight {
private:
	glm::vec3 m_Position; //position of the light
	glm::vec3 m_Color = glm::vec3(1.0f, 1.0f, 1.0f); //color of the light (default white)
	float a = 0.5; //quadratic term of intersity calc
	float b = 0.5; //linear term of intersity calc
public:
	PointLight();
	PointLight(glm::vec3 position): m_Position(position) {};
	PointLight(glm::vec3 position, glm::vec3 color) : m_Position(position), m_Color(color) {};
	~PointLight();
};
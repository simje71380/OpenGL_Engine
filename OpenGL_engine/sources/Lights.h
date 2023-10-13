#pragma once
#define MAX_POINT_LIGHTS 10

#include "glm/glm.hpp"
#include "Object3D.h"


class PointLight {
private:
public:
	glm::vec3 m_Position; //position of the light
	glm::vec3 m_Color = glm::vec3(1.0f, 1.0f, 1.0f); //color of the light (default white)
	float a = 0.5; //quadratic term of intersity calc
	float b = 0.5; //linear term of intersity calc

	PointLight(MaterialsManager* MM);
	PointLight(glm::vec3 position, MaterialsManager* MM);
	PointLight(glm::vec3 position, glm::vec3 color, MaterialsManager* MM);
	~PointLight();

	Object3D* obj;
	Material* material;
};
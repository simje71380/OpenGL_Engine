#pragma once

#include "Lights.h"

PointLight::PointLight(MaterialsManager* MM)
{
	//load pointlight mesh
	obj = new Object3D("res/models/pointlight.obj", true);
	m_Position = glm::vec3(0.0f, 0.0f, 0.0f);

	Shader* shader = MM->LoadShader("res/shaders/LightVertexShader.glsl", "res/shaders/LightFragShader.glsl"); //load light shader
	material = new Material(shader, true);
}

PointLight::PointLight(glm::vec3 position, MaterialsManager* MM)
	: m_Position(position)
{
	//load pointlight mesh
	obj = new Object3D("res/models/pointlight.obj", true);
	obj->Translate(position.x, position.y, position.z);

	Shader* shader = MM->LoadShader("res/shaders/LightVertexShader.glsl", "res/shaders/LightFragShader.glsl"); //load light shader
	material = new Material(shader, true);
}

PointLight::PointLight(glm::vec3 position, glm::vec3 color, MaterialsManager* MM)
	: m_Position(position), m_Color(color)
{
	//load pointlight mesh
	obj = new Object3D("res/models/pointlight.obj", true);
	obj->Translate(position.x, position.y, position.z);

	Shader* shader = MM->LoadShader("res/shaders/LightVertexShader.glsl", "res/shaders/LightFragShader.glsl"); //load light shader
	material = new Material(shader, true);
}

PointLight::~PointLight()
{
	delete obj;
	delete material;
}


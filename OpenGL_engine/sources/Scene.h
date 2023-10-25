#pragma once
#include "common/utils.h"
#include "Lights.h"
#include "Object3D.h"
#include "Camera.h"


//TODO: sceneLoader : take obj file from blender and generate all objs with name tag via o blabla
//TODO: saveSceane : saves a copy of the obj file and add model matrices

class Scene {
private : 
public:
	std::unordered_map<std::string, Object3D*> m_Objects;
	std::unordered_map<std::string, PointLight*> m_PointLights;
	std::unordered_map<std::string, Camera*> m_Cameras;
	Camera* main_camera = NULL;
	MaterialsManager MM;

	unsigned int maxPointLights = 10; //max pointlights given to frag shader -> taking the closest

	bool AddObject3D(Object3D* obj, std::string name) {
		// Check if key exists
		if (m_Objects.find(name) == m_Objects.end())
		{
			m_Objects.insert({ name, obj });
			return true;
		}
		else {
			std::cout << "Object : " << name << " already exist chose another one" << std::endl;
			return false;
		}
	};
	bool AddPointLight(PointLight* light, std::string name) {
		// Check if key exists
		if (m_PointLights.find(name) == m_PointLights.end())
		{
			m_PointLights.insert({ name, light });
			if (m_PointLights.size() > MAX_POINT_LIGHTS )
				std::cout << "there is mode than "<< MAX_POINT_LIGHTS <<" point lights some of them won't be used in rendering !!" << std::endl;
			return true;
		}
		else {
			std::cout << "Point light : " << name << " already exist chose another one" << std::endl;
			return false;
		}
	};
	bool AddCamera(Camera* cam, std::string name) {
		// Check if key exists
		if (m_Cameras.find(name) == m_Cameras.end())
		{
			m_Cameras.insert({ name, cam });
			if (main_camera == NULL)
				main_camera = cam;
			return true;
		}
		else {
			std::cout << "Camera : " << name << " already exist chose another one" << std::endl;
			return false;
		}
	};

	bool UseCamera(std::string name) {
		auto pos = m_Cameras.find(name);
		if (pos == m_Cameras.end()) { //unknown camera
			std::cout << "Camera : " << name << " doesn't exists" << std::endl;
			return false;
		}
		else {
			main_camera = pos->second;
			return true;
		}
	}

	bool LoadObjects(const char* file_path);

	~Scene() {
		for (std::unordered_map<std::string, Object3D*>::iterator it = m_Objects.begin(); it != m_Objects.end(); ++it)
		{
			delete it->second;
		}
	}
};
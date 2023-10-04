#pragma once
#include <unordered_map>
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"

class MaterialsManager {
private:
	std::unordered_map<std::string, Texture*> m_Textures;
	std::unordered_map<std::string, Shader*> m_Shaders;

	std::string concat(const char* one, const char* two);
public:
	MaterialsManager(){};
	~MaterialsManager();
	Texture* LoadTexture(const char* path);
	Shader* LoadShader(const char* vertex_path, const char* fragment_path);
};
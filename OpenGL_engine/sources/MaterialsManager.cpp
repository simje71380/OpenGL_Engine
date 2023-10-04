#pragma warning(disable:4996)
#include "MaterialsManager.h"

MaterialsManager::~MaterialsManager()
{
	//clear textures and shaders refs
    for (const std::pair<std::string, Texture*>& tup : m_Textures)
    {
        //clear memory
        delete tup.second;
    }

    for (const std::pair<std::string, Shader*>& tup : m_Shaders)
    {
        //clear memory
        delete tup.second;
    }
}

Texture* MaterialsManager::LoadTexture(const char* in_path)
{
    //formating string
    std::string path(in_path);

    auto pos = m_Textures.find(path);
    Texture* ptr;
    if (pos == m_Textures.end()) { //new path
        std::cout << path << " is not in the dictionary!" << std::endl;
        //loading the texture
        ptr = new Texture(path.c_str());
        m_Textures.insert({ path, ptr});
    }
    else {
        ptr = pos->second;
        std::cout << path << " is in the dictionary!" << std::endl;
    }
	return ptr;
}

Shader* MaterialsManager::LoadShader(const char* vertex_path, const char* fragment_path)
{
    const std::string paths = concat(vertex_path, fragment_path);
    auto pos = m_Shaders.find(paths);
    Shader* ptr;
    if (pos == m_Shaders.end()) { //new path
        std::cout << vertex_path << " ; " << fragment_path << " is not in the dictionary!" << std::endl;
        //loading the texture
        ptr = new Shader(vertex_path, fragment_path);
        m_Shaders.insert({paths, ptr});
    }
    else {
        ptr = pos->second;
        std::cout << paths << " is in the dictionary!" << std::endl;
    }
    return ptr;
}

std::string MaterialsManager::concat(const char* one, const char* two) {
    string i(one);
    string j(two);
    return i+j;
}

#pragma once
#include "Renderer.h"
#include "stb_image.h"
#include "common/utils.h"
#include <iostream>

class Texture {
private:
	int widthImg, heightImg, numColCh;
	GLuint texture;
public:
	Texture(const char* path);
	~Texture();
	void Bind();
	void Unbind();
};
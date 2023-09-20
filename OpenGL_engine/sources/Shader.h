#pragma once


#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;

#include <stdlib.h>
#include <string.h>
#include "Renderer.h"

class Shader {
private:
	GLuint ProgramID;
public:
	Shader(const char* vertex_path, const char* fragment_path);
	~Shader();
	void Bind();
	void Unbind();
	int GetUniformLocation(const char* name);
};
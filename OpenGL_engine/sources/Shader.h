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
#include "common/utils.h"

class Shader {
private:
	GLuint ProgramID;
public:
	~Shader();
	Shader(const char* vertex_path, const char* fragment_path);
	void Bind();
	void Unbind();
	int GetUniformLocation(const char* name);
	GLuint GetProgId() { return ProgramID; };
};
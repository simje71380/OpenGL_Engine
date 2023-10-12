#pragma once
#include <iostream>
#include <GL/glew.h>

//#define CUDA_ENABLED //use cuda computation

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);


std::string Get_Extension(std::string file_name);

std::string Change_File(const char* orignal_path, const char* new_file_name);
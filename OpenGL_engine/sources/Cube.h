#pragma once
#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Cube {
private:
    float vertices[48]{
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //avant bas gauche -> 0
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, //avant bas droit -> 1
         0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, //avant haut droit -> 2
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, //avant haut gauche -> 3
        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, //ARR bas gauche -> 4
         0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, //ARR bas droit -> 5
         0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, //ARR haut droit -> 6
        -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f  //ARR haut gauche -> 7
    };

    unsigned int indices[36]{
        0, 1, 2,
        2, 3, 0,
        4, 5, 6,
        6, 7, 4,
        2, 1, 5,
        5, 6, 2,
        0, 3, 7,
        7, 4, 0,
        0, 1, 5,
        5, 4, 0,
        3, 2, 6,
        6, 7, 3 
    };

    VertexArray* va;
    VertexBuffer* vb;
    IndexBuffer* ib;
    Shader* shader;
public:
	Cube();
	~Cube();
	
	void Draw();
};
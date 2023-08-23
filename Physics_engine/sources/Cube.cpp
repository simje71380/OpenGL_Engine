#include "Cube.h"


Cube::Cube()
{
    //setting up the vertex array object
    va = new VertexArray;

    //setting up vertex buffer
    vb = new VertexBuffer(vertices, 6 * 8 * sizeof(float));

    //setting up the VB layout
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(3);
    va->AddBuffer(*vb, layout);

    //setting up index buffer
    ib = new IndexBuffer(indices, 24);

    //shading
    shader = LoadShaders("res/shaders/3DVertexShader.glsl", "res/shaders/3DFragShader.glsl");
    GLCall(glUseProgram(shader)); //set drawing with this shader (vertex + frag)
}

Cube::~Cube()
{
    delete vb;
    delete ib;
    delete va;
}

void Cube::Draw()
{
    GLCall(glUseProgram(shader)); //set drawing with this shader (vertex + frag)

    va->Bind();
    ib->Bind();
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); //with index buffer   6->number of indices

}

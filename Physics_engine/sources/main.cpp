#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "common/shader.hpp"


int main(void)
{
    GLFWwindow* window;
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    /*Triangle triangle("shaders/SimpleVertexShader.glsl", "shaders/SimpleFragmentShader.glsl");
    Triangle triangle2("shaders/SimpleVertexShader.glsl", "shaders/SimpleFragmentShader2.glsl");*/

    float positions[8]{
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    unsigned int indices[]{
        0, 1, 2,
        2, 3, 0
    };

    unsigned int bufferID; //ID of a vertex buffer
    glGenBuffers(1, &bufferID); //create one buffer and return the ID into bufferID
    glBindBuffer(GL_ARRAY_BUFFER, bufferID); //say to open GL we are talk about the "bufferID" buffer
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); //active le layout du vertex
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); //specify how is the layout of the vertex (coords, UV coords,...)

    unsigned int ibo; //ID of index buffer
    glGenBuffers(1, &ibo); //create one buffer and return the ID into ibo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //say to open GL we are talk about the "ibo" buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);


    //shading
    unsigned int prog_id = LoadShaders("res/shaders/SimpleVertexShader.glsl", "res/shaders/SimpleFragmentShader.glsl");
    glUseProgram(prog_id); //set drawing with this shader (vertex + frag)


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT); // clear screen


        //glBindBuffer(GL_ARRAY_BUFFER, 0); //bind to None -> nothing will be drawn
        //glDrawArrays(GL_TRIANGLES, 0, 3); //without index buffer
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); //with index buffer   6->number of indices

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
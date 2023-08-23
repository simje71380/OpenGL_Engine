#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "common/shader.hpp"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Cube.h"
#include "Object3D.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(void)
{
    float width = 1024;
    float height = 768;
    GLFWwindow* window;
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    std::cout << glGetString(GL_VERSION) << std::endl;

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, width, height);
    Camera camera(width, height);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /*Triangle triangle("shaders/SimpleVertexShader.glsl", "shaders/SimpleFragmentShader.glsl");
    Triangle triangle2("shaders/SimpleVertexShader.glsl", "shaders/SimpleFragmentShader2.glsl");*/
    {
        /*float positions[]{
            -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f, 1.0f, 1.0f, 1.0f
        };

        unsigned int indices[]{
            0, 1, 2,
            2, 3, 0
        };

        //setting up the vertex array object
        VertexArray va;

        //setting up vertex buffer
        VertexBuffer vb(positions, 4 * 5 * sizeof(float));

        //setting up the VB layout
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(3);
        va.AddBuffer(vb, layout);

        //setting up index buffer
        IndexBuffer ib(indices, 6);

        //shading
        unsigned int shader = LoadShaders("res/shaders/SimpleVertexShader.glsl", "res/shaders/SimpleFragmentShader.glsl");
        GLCall(glUseProgram(shader)); //set drawing with this shader (vertex + frag)

        GLCall(int location = glGetUniformLocation(shader, "u_Color")); //get the location of the uniform in the shader
        ASSERT(location != -1); //verify error
        */
        float r = 0.0f;
        float increment = 0.05f;

        Vertex* vertices = new Vertex[8]{
            Vertex(-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
            Vertex( 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
            Vertex(-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f),
            Vertex( 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f),
            Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
            Vertex( 0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
            Vertex(-0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f),
            Vertex( 0.5f,  0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f)
        };

        unsigned int *indices = new unsigned int[36]{
             2, 6, 7,
             2, 3, 7,
             0, 4, 5,
             0, 1, 5,
             0, 2, 6,
             0, 4, 6,
             1, 3, 7,
             1, 5, 7,
             0, 2, 3,
             0, 1, 3,
             4, 6, 7,
             4, 5, 7
        };

        Object3D obj(8, vertices, indices, 36);

        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen

            int width, height;
            GLCall(glfwGetWindowSize(window, &width, &height));
            camera.Resize(width, height);

            //glBindBuffer(GL_ARRAY_BUFFER, 0); //bind to None -> nothing will be drawn
            //glDrawArrays(GL_TRIANGLES, 0, 3); //without index buffer

            /*
            GLCall(glUseProgram(shader)); //set drawing with this shader (vertex + frag)
            GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f)); //setting the uniform

            va.Bind();
            ib.Bind();
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); //with index buffer   6->number of indices
            */
            
            //c.Draw();
            obj.Rotate(0.01f, 0.01f, 0.01f);
            obj.Draw(&camera);

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
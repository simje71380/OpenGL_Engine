#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Cube.h"
#include "Object3D.h"
#include "Texture.h"
#include "Shader.h"
#include "MaterialsManager.h"
#include "Lights.h"
#include "Scene.h"


//CUDA kernels DLL
#include "../CUDA/CudaKernel.h"

//memory analysis
#include "windows.h"
#define _CRTDBG_MAP_ALLOC //to get more details
#include <crtdbg.h>   //for malloc and free

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(void)
{
    //memory analysis
    _CrtMemState sOld;
    _CrtMemState sNew;
    _CrtMemState sDiff;
    _CrtMemCheckpoint(&sOld); //take a snapshot

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
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    {
        Vertex* vertices = new Vertex[8]{
            Vertex(glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
            Vertex(glm::vec4( 0.5f, -0.5f, -0.5f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
            Vertex(glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
            Vertex(glm::vec4( 0.5f,  0.5f, -0.5f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
            Vertex(glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
            Vertex(glm::vec4( 0.5f, -0.5f,  0.5f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
            Vertex(glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
            Vertex(glm::vec4( 0.5f,  0.5f,  0.5f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),

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

        //Object3D obj(8, vertices, indices, 36, mat);

        Renderer renderer;
        Scene scene;

        MaterialsManager MM;
        
        scene.LoadObjects("res/models/sphere.obj");

        PointLight pointLight(glm::vec3(-1.1f, 2.5f, 0.9f), &MM);
        pointLight.a = 1;
        pointLight.b = 0.3;
        scene.AddPointLight(&pointLight, "point light 1");
        PointLight pointLight2(glm::vec3(1.0f, -1.5f, 1.0f), glm::vec3(0.1f, 1.0f, 1.0f), &MM);
        pointLight2.a = 1;
        pointLight2.b = 0.3;
        scene.AddPointLight(&pointLight2, "point light 2");
        scene.AddCamera(&camera, "main camera");

        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);

        //FPS counter
        double init_time = glfwGetTime();
        double prevTime = 0.0;
        double crntTime = 0.0;
        double deltaTime;
        double fps_delta = 0.0;
        unsigned int counter = 0;
        unsigned long long image_count = 0;

        //disable Vsync
        //glfwSwapInterval(0);

        double rotation_speed = 1.0;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            //FPS counter
            crntTime = glfwGetTime();
            deltaTime = crntTime - prevTime;
            fps_delta += deltaTime;
            counter++;
            image_count++;
            if (fps_delta > 1.0 / 5.0) { //rafraichisssement du compteur a 5hz
                std::string FPS = std::to_string((1.0 / fps_delta) * counter);
                std::string AVG = std::to_string((1.0 / (crntTime - init_time)) * image_count);
                std::string newTitle = "Engine - " + FPS + " FPS \ AVG : " + AVG + " FPS";
                glfwSetWindowTitle(window, newTitle.c_str());
                fps_delta = 0.0;
                counter = 0;
            }
            
            prevTime = crntTime;


            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen

            int width, height;
            GLCall(glfwGetWindowSize(window, &width, &height));
            camera.Resize(width, height);
            camera.Inputs(window, deltaTime); //deltaTime allow to have the same amount of movement independantly from FPS

            //glBindBuffer(GL_ARRAY_BUFFER, 0); //bind to None -> nothing will be drawn
            //glDrawArrays(GL_TRIANGLES, 0, 3); //without index buffer

            /*
            shader.Bind(); //set drawing with this shader (vertex + frag)
            
            texture.Bind();
            va.Bind();
            ib.Bind();
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); //with index buffer   6->number of indices
            */
            
            //c.Draw();
            /*obj.Rotate(rotation_speed * deltaTime, rotation_speed * deltaTime, rotation_speed * deltaTime);
            */
            //obj.Draw(&camera);
            renderer.Render(&scene);
            /*
            sphere.Rotate(0.0f, 0.0f, rotation_speed * deltaTime);
            float x = 1+sin(crntTime) * deltaTime;
            //sphere.Scale(x);
            //sphere.Translate(1 * deltaTime, 0.0f, 0.0f);
            sphere.Draw(&camera);
            */


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();

    //check for memory leaks
    _CrtMemCheckpoint(&sNew); //take a snapshot 
    if (_CrtMemDifference(&sDiff, &sOld, &sNew)) // if there is a difference
    {
        std::cout << "\n\nMEMORY LEAKS DETECTED !!!\n\n" << std::endl;
        OutputDebugString(L"\n -----------_CrtMemDumpStatistics ---------\n");
        _CrtMemDumpStatistics(&sDiff);
        OutputDebugString(L"\n -----------_CrtMemDumpAllObjectsSince ---------\n");
        _CrtMemDumpAllObjectsSince(&sOld);
        OutputDebugString(L"\n -----------_CrtDumpMemoryLeaks ---------\n");
        _CrtDumpMemoryLeaks();
    }
    else {
        std::cout << "\n\nNo memory leaks detected\n\n" << std::endl;
        OutputDebugString(L"\n ----------- No leaks --------- \n\n");
    }

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
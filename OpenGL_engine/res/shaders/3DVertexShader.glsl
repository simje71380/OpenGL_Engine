#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 UVs;
layout(location = 2) in vec3 normals;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

out vec3 fNormals;
out vec3 FragPos;

void main(){
    gl_Position = Proj * View * Model * pos;

    fNormals = inverse(transpose(mat3(Model))) * normals;  

    FragPos = vec3(Model * pos);
}


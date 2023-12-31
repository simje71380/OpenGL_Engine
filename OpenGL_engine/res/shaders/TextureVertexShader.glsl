#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 UVs;
layout(location = 2) in vec3 normals;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

out vec4 fColor;
out vec3 fNormals;
out vec3 FragPos;
out vec2 TexCoord;

void main(){
    gl_Position = Proj * View * Model * pos;
    fColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);

    fNormals = inverse(transpose(mat3(Model))) * normals;  
    TexCoord = UVs;
    FragPos = vec3(Model * pos);
}
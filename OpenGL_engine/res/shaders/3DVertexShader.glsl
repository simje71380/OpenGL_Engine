#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 UVs;
layout(location = 2) in vec3 normals;

uniform mat4 MVP;

out vec4 fColor;
out vec3 fNormals;

void main(){
    fColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    fNormals = normals;
    gl_Position = MVP * vec4(pos);
}


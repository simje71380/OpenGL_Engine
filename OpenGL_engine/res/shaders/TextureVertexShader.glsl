#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 UVs;
layout(location = 2) in vec3 normals;

uniform mat4 MVP;
uniform vec3 Ka;

out vec4 fColor;
out vec2 TexCoord;
out vec3 fKa;

void main(){
    fColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    TexCoord = UVs;
    fKa = Ka;
    gl_Position = MVP * vec4(pos);
}

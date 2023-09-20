#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 coords;

out vec4 fColor;
out vec2 texCoord;

void main(){
    fColor = vec4(color, 1.0f);
    texCoord = coords;
    gl_Position = vec4(pos, 1.0f);
}


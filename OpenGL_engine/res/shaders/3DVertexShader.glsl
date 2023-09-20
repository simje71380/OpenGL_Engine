#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 color;

uniform mat4 MVP;

out vec4 fColor;

void main(){
    fColor = vec4(color);
    
    gl_Position = MVP * vec4(pos);
}


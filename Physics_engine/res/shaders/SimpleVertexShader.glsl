#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 pos;
layout(location = 1) in vec3 color;

out vec4 fColor;

void main(){
    fColor = vec4(color, 1.0f);
    
    gl_Position = vec4(pos ,1.0f, 1.0f);
}


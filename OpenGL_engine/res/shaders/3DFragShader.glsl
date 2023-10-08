#version 330 core

layout(location = 0) out vec4 color;

in vec4 fColor;
in vec3 fNormals;
in vec3 FragPos;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform vec3 ViewPos;

void main()
{
    //to pass as uniform
    vec3 lightPos = vec3(0.0, 3.0, 0.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    
    //ambiant
    float ambiantStrength = 0.1f;
    vec3 ambient = ambiantStrength * lightColor;

    //diffuse
    vec3 norm = normalize(fNormals);
    vec3 lightDir = normalize(lightPos - FragPos);  
    float diffuseStrength = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * lightColor;

    //specular
    float specularStrength = 1.5;
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir) , 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = ambient * Ka + diffuse * Kd + specular * Ks;
    color = vec4(result, 1.0);
};
#version 330 core

layout(location = 0) out vec4 color;

in vec4 fColor;
in vec2 TexCoord;
in vec3 fNormals;
in vec3 FragPos;

uniform sampler2D Texture;

uniform vec3 ViewPos;

uniform vec3 Ks;

struct PointLight{
    vec3 pos;
    vec3 color;
	float a;
	float b;
};

uniform PointLight pointLights[10];

vec3 pointLight(int i)
{	
	if(pointLights[i].a == -1){
		return vec3(0.0f, 0.0f, 0.0f);
	}

	// used in two variables so I calculate it here to not have to do it twice
	vec3 lightVec = pointLights[i].pos - FragPos;

	// intensity of light with respect to distance
	float dist = length(lightVec);
	float inten = 1.0f / (pointLights[i].a * dist * dist + pointLights[i].b * dist + 1.0f);

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(fNormals);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = Ks.x;
	vec3 viewDirection = normalize(ViewPos - FragPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return vec3(texture(Texture, TexCoord)) * (ambient + diffuse * inten + specular * inten) * pointLights[i].color;
}


void main()
{
	for(int i = 0; i < 10; i++)
		color += vec4(pointLight(i), 1.0f);
};
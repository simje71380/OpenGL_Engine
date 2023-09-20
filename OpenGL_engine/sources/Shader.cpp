#include "Shader.h"

Shader::Shader(const char* vertex_path, const char* fragment_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_path);
		getchar();
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	GLCall(glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL));
	GLCall(glCompileShader(VertexShaderID));

	// Check Vertex Shader
	GLCall(glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result));
	GLCall(glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength));
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		GLCall(glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]));
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	GLCall(glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL));
	GLCall(glCompileShader(FragmentShaderID));

	// Check Fragment Shader
	GLCall(glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result));
	GLCall(glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength));
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		GLCall(glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]));
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	ProgramID = glCreateProgram();
	GLCall(glAttachShader(ProgramID, VertexShaderID));
	GLCall(glAttachShader(ProgramID, FragmentShaderID));
	GLCall(glLinkProgram(ProgramID));

	// Check the program
	GLCall(glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result));
	GLCall(glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength));
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		GLCall(glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]));
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	GLCall(glDetachShader(ProgramID, VertexShaderID));
	GLCall(glDetachShader(ProgramID, FragmentShaderID));

	GLCall(glDeleteShader(VertexShaderID));
	GLCall(glDeleteShader(FragmentShaderID));

	GLCall(glUseProgram(0));
}

Shader::~Shader()
{
}

void Shader::Bind()
{
	GLCall(glUseProgram(ProgramID));
}

void Shader::Unbind()
{
	GLCall(glUseProgram(0));
}

int Shader::GetUniformLocation(const char* name)
{
	GLCall(int location = glGetUniformLocation(ProgramID, name)); //get the location of the uniform in the shader
	ASSERT(location != -1); //verify error
	return location;
}

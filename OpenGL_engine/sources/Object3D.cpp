#pragma warning(disable:4996)

#include "Object3D.h"

Object3D::Object3D(unsigned int vertices_count, Vertex *vertices, unsigned int *indices, unsigned int indices_count, Material *material)
	: m_Vertices_Count(vertices_count), m_Vertices(vertices){

	//setting up indicies and materials
	index_mat wrapper;
	wrapper.m_Indices_Count = indices_count;
	wrapper.m_Indices = indices;
	wrapper.m_mat = material;
	//setting up index buffer
	wrapper.m_ib = new IndexBuffer(indices, indices_count);
	
	wrapper.m_mat->Bind(); //set drawing with this shader (vertex + frag)

	m_MaterialWrapper.push_back(wrapper);


	//setting up the vertex array object
	m_va = new VertexArray;

	//setting up vertex buffer
	m_vb = new VertexBuffer(m_Vertices, m_Vertices_Count * sizeof(Vertex));

	//setting up the VB layout
	m_layout = new VertexBufferLayout;
	m_layout->Push<float>(4);
	m_layout->Push<float>(2);
	m_layout->Push<float>(3);
	m_va->AddBuffer(*m_vb, *m_layout);
}

Object3D::~Object3D()
{
	delete[] m_Vertices;
	delete m_va;
	delete m_vb;
	delete m_layout;

	for (unsigned int i = 0; i < m_MaterialWrapper.size(); i++) {
		delete[] m_MaterialWrapper[i].m_Indices;
		delete m_MaterialWrapper[i].m_ib;
	}
	for (unsigned int i = 0; i < materials.size(); i++) {
		delete materials[i];
	}
}

//loading from obj file
Object3D::Object3D(const char* file_path, MaterialsManager* MM)
{
	//verify extension
	if (Get_Extension(file_path) != ".obj") {
		std::cout << "Wrong file extension must be .obj ! current :" << Get_Extension(file_path) << std::endl;
		return;
	}

	//open file
	FILE* file = fopen(file_path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return;
	}

	//parsing file
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	std::vector<Vertex> vertices;
	char material_file[128]; //mtllib file
	char material_name[128];

	unsigned int index_number = 0;
	std::vector<std::vector<unsigned int>> all_indicies;
	std::vector<unsigned int> local_indicies;
	std::vector<std::string> material_names;
	unsigned int depth = 0;

	while (1) {
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;
		if (strcmp(lineHeader, "mtllib") == 0){
			//open and analyse the material file
			fscanf(file, "%s\n", &material_file);
			materials = LoadMaterial(Change_File(file_path, material_file).c_str(), MM);
		}
		else if (strcmp(lineHeader, "usemtl") == 0) { //new materials
			if (index_number != 0)//avoid push the first apparition
				all_indicies.push_back(local_indicies);
			local_indicies.clear();//clear buffer
			//get name of material
			fscanf(file, "%s\n", &material_name);
			material_names.push_back(material_name);
		}
		else if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return;
			}
			//creer les vertices
			vertices.push_back(Vertex(temp_vertices[vertexIndex[0]-1], temp_uvs[uvIndex[0]-1], temp_normals[normalIndex[0]-1]));
			vertices.push_back(Vertex(temp_vertices[vertexIndex[1]-1], temp_uvs[uvIndex[1]-1], temp_normals[normalIndex[1]-1]));
			vertices.push_back(Vertex(temp_vertices[vertexIndex[2]-1], temp_uvs[uvIndex[2]-1], temp_normals[normalIndex[2]-1]));
			
			local_indicies.push_back(index_number);
			local_indicies.push_back(index_number+1);
			local_indicies.push_back(index_number+2);
			index_number += 3;
		}
	}
	//pushback last index vector
	all_indicies.push_back(local_indicies);
	local_indicies.clear();


	//processing the data

	//setup vertex buffer
	m_Vertices = new Vertex[vertices.size()];
	m_Vertices_Count = vertices.size();

	for (unsigned int i = 0; i < vertices.size(); i++) {
		m_Vertices[i].Set(vertices[i]);
	}

	//setting up the vertex array object
	m_va = new VertexArray;

	//setting up vertex buffer
	m_vb = new VertexBuffer(m_Vertices, m_Vertices_Count * sizeof(Vertex));

	//setting up the VB layout
	m_layout = new VertexBufferLayout;
	m_layout->Push<float>(4); //pos
	m_layout->Push<float>(2); //UV
	m_layout->Push<float>(3); //normals
	m_va->AddBuffer(*m_vb, *m_layout);

	//setup indexbuffers with their materials and shaders
	for (unsigned int i = 0; i < all_indicies.size();i++) {
		//setting up index buffer
		index_mat wrapper;

		wrapper.m_Indices = new unsigned int[all_indicies[i].size()];
		unsigned int indices_count = all_indicies[i].size();

		for (unsigned int j = 0; j < indices_count; j++) {
			wrapper.m_Indices[j] = all_indicies[i][j];
		}

		wrapper.m_Indices_Count = indices_count;
		wrapper.m_ib = new IndexBuffer(wrapper.m_Indices, indices_count);

		//bind material
		for (unsigned int j = 0; j < materials.size(); j++) {
			if (material_names[i] == (std::string)materials[j]->name) {
				wrapper.m_mat = materials[i];
			}
		}
		//wrapper.m_mat->Bind(); //set the material
		m_MaterialWrapper.push_back(wrapper);
	}
}

void Object3D::Rotate(float theta_x, float theta_y, float theta_z)
{
	//proceed the rotation to the vertex
	glm::mat4 RMatrix_x = glm::rotate(theta_x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 RMatrix_y = glm::rotate(theta_y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 RMatrix_z = glm::rotate(theta_z, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 RMatrix = RMatrix_x * RMatrix_y * RMatrix_z;

	for (unsigned int i = 0; i < m_Vertices_Count; i++) {
		Vertex v(m_Vertices[i]);
		glm::vec4 myVector(v.position);
		m_Vertices[i].position = RMatrix * myVector;
	}

	//update the vertex buffer
	m_vb->Update(m_Vertices, m_Vertices_Count * sizeof(Vertex));
	m_va->AddBuffer(*m_vb, *m_layout);
}

void Object3D::Translate(float x, float y, float z)
{
	//proceed the translation to the vertex
	glm::mat4 TMatrix = glm::translate(glm::mat4(1), glm::vec3(x, y, z));
	for (unsigned int i = 0; i < m_Vertices_Count; i++) {
		Vertex v(m_Vertices[i]);
		glm::vec4 myVector(v.position);
		m_Vertices[i].position = TMatrix * myVector;
	}

	//update the vertex buffer
	m_vb->Update(m_Vertices, m_Vertices_Count * sizeof(Vertex));
	m_va->AddBuffer(*m_vb, *m_layout);
}

void Object3D::Scale(float x, float y, float z)
{
	//proceed the scaling to the vertex
	glm::mat4 SMatrix = glm::scale(glm::vec3(x, y, z));
	for (unsigned int i = 0; i < m_Vertices_Count; i++) {
		Vertex v(m_Vertices[i]);
		glm::vec4 myVector(v.position);
		m_Vertices[i].position = SMatrix * myVector;
	}

	//update the vertex buffer
	m_vb->Update(m_Vertices, m_Vertices_Count * sizeof(Vertex));
	m_va->AddBuffer(*m_vb, *m_layout);
}

void Object3D::Draw(Camera* cam)
{
	m_va->Bind();
	for (unsigned int i = 0; i < m_MaterialWrapper.size(); i++) {
		m_MaterialWrapper[i].m_mat->Bind(); //set the material
		m_MaterialWrapper[i].m_ib->Bind(); //set the indexbuffer

		//getting the MVP matrix
		glm::mat4 mvp = cam->Get_MVP(m_Model);

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
		GLCall(glUniformMatrix4fv(m_MaterialWrapper[i].m_mat->m_MatrixID, 1, GL_FALSE, &mvp[0][0]));
		GLCall(glUniform3f(m_MaterialWrapper[i].m_mat->m_KaID, m_MaterialWrapper[i].m_mat->Ka.x, m_MaterialWrapper[i].m_mat->Ka.r, m_MaterialWrapper[i].m_mat->Ka.s));


		GLCall(glDrawElements(GL_TRIANGLES, m_MaterialWrapper[i].m_Indices_Count, GL_UNSIGNED_INT, nullptr));
	}
}

std::vector<Material*> Object3D::LoadMaterial(const char* file_path, MaterialsManager* MM) {
	std::vector<Material*> output;

	FILE* file = fopen(file_path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return output;
	}

	Shader* shader = MM->LoadShader("res/shaders/3DVertexShader.glsl", "res/shaders/3DFragShader.glsl"); //load default shader
	Material* temp_material = new Material(shader);
	unsigned int mat_num = 0;

	while (1) {
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "newmtl") == 0) {
			if (mat_num != 0) { //avoid 1st read push
				output.push_back(temp_material);
			}
			temp_material = new Material(shader);

			char material_name[128];
			fscanf(file, "%s\n", &material_name); //get name
			for (int i = 0; i < 128; i++) {
				temp_material->name[i] = material_name[i];
			}
			mat_num++;
		}
		else if (strcmp(lineHeader, "Ns") == 0) {
			float Ns;
			fscanf(file, "%f\n", &Ns);
			temp_material->Ns = Ns;
		}
		else if (strcmp(lineHeader, "Ka") == 0) {
			glm::vec3 Ka;
			fscanf(file, "%f, %f, %f\n", &Ka.x, &Ka.y, &Ka.z);
			temp_material->Ka = Ka;
		}
		else if (strcmp(lineHeader, "Kd") == 0) {
			glm::vec3 Kd;
			fscanf(file, "%f, %f, %f\n", &Kd.x, &Kd.y, &Kd.z);
			temp_material->Kd = Kd;
		}
		else if (strcmp(lineHeader, "Ks") == 0) {
			glm::vec3 Ks;
			fscanf(file, "%f, %f, %f\n", &Ks.x, &Ks.y, &Ks.z);
			temp_material->Ks = Ks;
		}
		else if (strcmp(lineHeader, "Ke") == 0) {
			glm::vec3 Ke;
			fscanf(file, "%f, %f, %f\n", &Ke.x, &Ke.y, &Ke.z);
			temp_material->Ke = Ke;
		}
		else if (strcmp(lineHeader, "Ni") == 0) {
			float Ni;
			fscanf(file, "%f\n", &Ni);
			temp_material->Ni = Ni;
		}
		else if (strcmp(lineHeader, "d") == 0) {
			float d;
			fscanf(file, "%f\n", &d);
			temp_material->d = d;
		}
		else if (strcmp(lineHeader, "illum") == 0) {
			float illum;
			fscanf(file, "%f\n", &illum);
			temp_material->illum = illum;
		}
		else if (strcmp(lineHeader, "map_Kd") == 0) {
			//texture
			char filename[128];
			fscanf(file, "%s\n", &filename);
			temp_material->m_Texture = MM->LoadTexture(Change_File(file_path, filename).c_str());
			//switch to textured shader
			temp_material->m_Shader = MM->LoadShader("res/shaders/TextureVertexShader.glsl", "res/shaders/TextureFragShader.glsl");
		}
	}

	//push last material
	output.push_back(temp_material);

	return output;
}
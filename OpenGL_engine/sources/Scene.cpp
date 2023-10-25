#pragma warning(disable:4996)

#include "Scene.h"

std::vector<Material*> LoadMaterial(const char* file_path, MaterialsManager* MM);

struct object_data {
	std::string name;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	std::vector<Vertex> vertices;

	unsigned int index_number = 0;
	std::vector<std::vector<unsigned int>> all_indicies;
	std::vector<unsigned int> local_indicies;
	std::vector<std::string> material_names;
};

bool Scene::LoadObjects(const char* file_path) {
	//verify extension
	if (Get_Extension(file_path) != ".obj") {
		std::cout << "Wrong file extension must be .obj ! current :" << Get_Extension(file_path) << std::endl;
		return false;
	}

	//open file
	FILE* file = fopen(file_path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}

	//same variables for all objects in the file
	char material_file[128]; //mtllib file
	std::vector<Material*> materials; //All materials from the mtllib file

	//temporary variables
	char material_name[128];
	unsigned int object_index = -1; //start to -1 -> fisrt "o" appartition index++
	object_data obj;

	std::vector<object_data> objects_data;
	while (1) {
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;
		if (strcmp(lineHeader, "mtllib") == 0) {
			//open and analyse the material file
			fscanf(file, "%s\n", &material_file);
			materials = LoadMaterial(Change_File(file_path, material_file).c_str(), &MM);
		}
		else if (strcmp(lineHeader, "o") == 0) {
			//get object name
			object_index++;
			if (object_index > 0) {
				obj.all_indicies.push_back(obj.local_indicies);
				objects_data.push_back(obj);
			}

			//clear object
			obj.name = "";
			//obj.temp_vertices.clear();
			//obj.temp_uvs.clear();
			//obj.temp_normals.clear();
			obj.vertices.clear();
			obj.index_number = 0;
			obj.all_indicies.clear();
			obj.local_indicies.clear();
			obj.material_names.clear();

			//set new obj name
			char obj_name[128];
			fscanf(file, "%s\n", &obj_name);
			obj.name = obj_name;
		}
		else if (strcmp(lineHeader, "usemtl") == 0) { //new materials
			if (obj.index_number > 0)//avoid push the first apparition
				obj.all_indicies.push_back(obj.local_indicies);

			obj.local_indicies.clear();//clear buffer
			//get name of material
			fscanf(file, "%s\n", &material_name);
			obj.material_names.push_back(material_name);
		}
		else if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			obj.temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			obj.temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			obj.temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			//creer les vertices
			obj.vertices.push_back(Vertex(obj.temp_vertices[vertexIndex[0] - 1], obj.temp_uvs[uvIndex[0] - 1], obj.temp_normals[normalIndex[0] - 1]));
			obj.vertices.push_back(Vertex(obj.temp_vertices[vertexIndex[1] - 1], obj.temp_uvs[uvIndex[1] - 1], obj.temp_normals[normalIndex[1] - 1]));
			obj.vertices.push_back(Vertex(obj.temp_vertices[vertexIndex[2] - 1], obj.temp_uvs[uvIndex[2] - 1], obj.temp_normals[normalIndex[2] - 1]));

			obj.local_indicies.push_back(obj.index_number);
			obj.local_indicies.push_back(obj.index_number + 1);
			obj.local_indicies.push_back(obj.index_number + 2);
			obj.index_number += 3;
		}
	}
	//pushback last index vector
	obj.all_indicies.push_back(obj.local_indicies);
	obj.local_indicies.clear();
	objects_data.push_back(obj); //push last generated obj


	//processing the data
	for (object_data obj_data : objects_data) {
		Object3D* object = new Object3D();
		
		//object.materials = materials;

		//setup vertex buffer
		object->m_Vertices = new Vertex[obj_data.vertices.size()];
		object->m_Vertices_Count = obj_data.vertices.size();

		for (unsigned int i = 0; i < obj_data.vertices.size(); i++) {
			object->m_Vertices[i].Set(obj_data.vertices[i]);
		}

		//setting up the vertex array object
		object->m_va = new VertexArray;

		//setting up vertex buffer
		object->m_vb = new VertexBuffer(object->m_Vertices, object->m_Vertices_Count * sizeof(Vertex));

		//setting up the VB layout
		object->m_layout = new VertexBufferLayout;
		object->m_layout->Push<float>(4); //pos
		object->m_layout->Push<float>(2); //UV
		object->m_layout->Push<float>(3); //normals
		object->m_va->AddBuffer(*object->m_vb, *object->m_layout);

		//setup indexbuffers with their materials and shaders
		for (unsigned int i = 0; i < obj_data.all_indicies.size();i++) {
			//setting up index buffer
			index_mat wrapper;

			wrapper.m_Indices = new unsigned int[obj_data.all_indicies[i].size()];
			unsigned int indices_count = obj_data.all_indicies[i].size();

			for (unsigned int j = 0; j < indices_count; j++) {
				wrapper.m_Indices[j] = obj_data.all_indicies[i][j];
			}

			wrapper.m_Indices_Count = indices_count;
			wrapper.m_ib = new IndexBuffer(wrapper.m_Indices, indices_count);

			//bind material
			for (unsigned int j = 0; j < materials.size(); j++) {
				if (obj_data.material_names[i] == (std::string)materials[j]->name) {
					wrapper.m_mat = materials[j];
				}
			}
			//wrapper.m_mat->Bind(); //set the material
			object->m_MaterialWrapper.push_back(wrapper);
		}
		AddObject3D(object, obj_data.name);
	}
	return true;
}

std::vector<Material*> LoadMaterial(const char* file_path, MaterialsManager* MM) {
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
			fscanf(file, "%f %f %f\n", &Ka.x, &Ka.y, &Ka.z);
			temp_material->Ka = Ka;
		}
		else if (strcmp(lineHeader, "Kd") == 0) {
			glm::vec3 Kd;
			fscanf(file, "%f %f %f\n", &Kd.x, &Kd.y, &Kd.z);
			temp_material->Kd = Kd;
		}
		else if (strcmp(lineHeader, "Ks") == 0) {
			glm::vec3 Ks;
			fscanf(file, "%f %f %f\n", &Ks.x, &Ks.y, &Ks.z);
			temp_material->Ks = Ks;
		}
		else if (strcmp(lineHeader, "Ke") == 0) {
			glm::vec3 Ke;
			fscanf(file, "%f %f %f\n", &Ke.x, &Ke.y, &Ke.z);
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
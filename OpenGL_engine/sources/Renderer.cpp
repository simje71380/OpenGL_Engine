#include "Renderer.h"

struct PLight {
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
	float a = -1.0f;
	float b = -1.0f;
};

void Renderer::Render(Scene* scene)
{
	PLight pointLights[MAX_POINT_LIGHTS];
	int i = 0;
	//TODO : take the closest lights to the camera in the correct direction
	for (const std::pair<std::string, PointLight*>& pair : scene->m_PointLights) {
		pointLights[i].pos = pair.second->m_Position;
		pointLights[i].color = pair.second->m_Color;
		pointLights[i].a = pair.second->a;
		pointLights[i].a = pair.second->b;
		i++;

		//render the light
		Object3D* object = pair.second->obj;
		Material* m_mat = pair.second->material;
		object->m_va->Bind();

		m_mat->Bind(); //set the material
		object->m_MaterialWrapper[0].m_ib->Bind(); //set the indexbuffer

		glm::mat4 model = glm::translate(glm::mat4(1.0f), object->m_Origin);
		model *= object->m_Model; //rotation + scale

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
		GLCall(glUniformMatrix4fv(m_mat->m_ViewMatrixID, 1, GL_FALSE, &scene->main_camera->Get_View()[0][0]));
		GLCall(glUniformMatrix4fv(m_mat->m_ProjMatrixID, 1, GL_FALSE, &scene->main_camera->Get_Proj()[0][0]));
		GLCall(glUniformMatrix4fv(m_mat->m_ModelMatrixID, 1, GL_FALSE, &model[0][0]));

		//light color uniform
		std::string lightColor_str = "lightColor";
		GLCall(glUniform3f(m_mat->m_Shader->GetUniformLocation(lightColor_str.c_str()),
			pair.second->m_Color.x,
			pair.second->m_Color.y,
			pair.second->m_Color.z
		));

		GLCall(glDrawElements(GL_TRIANGLES, object->m_MaterialWrapper[0].m_Indices_Count, GL_UNSIGNED_INT, nullptr));
}

	for(const std::pair<std::string, Object3D*>& pair : scene->m_Objects){
		Object3D* object = pair.second;
		object->m_va->Bind();
		for (unsigned int mat_idx = 0; mat_idx < object->m_MaterialWrapper.size(); mat_idx++) {
			object->m_MaterialWrapper[mat_idx].m_mat->Bind(); //set the material
			object->m_MaterialWrapper[mat_idx].m_ib->Bind(); //set the indexbuffer

			glm::mat4 model = glm::translate(glm::mat4(1.0f), object->m_Origin);
			model *= object->m_Model; //rotation + scale

			// Send our transformation to the currently bound shader, in the "MVP" uniform
			// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
			GLCall(glUniformMatrix4fv(object->m_MaterialWrapper[mat_idx].m_mat->m_ViewMatrixID, 1, GL_FALSE, &scene->main_camera->Get_View()[0][0]));
			GLCall(glUniformMatrix4fv(object->m_MaterialWrapper[mat_idx].m_mat->m_ProjMatrixID, 1, GL_FALSE, &scene->main_camera->Get_Proj()[0][0]));
			GLCall(glUniformMatrix4fv(object->m_MaterialWrapper[mat_idx].m_mat->m_ModelMatrixID, 1, GL_FALSE, &model[0][0]));

			GLCall(glUniform3f(object->m_MaterialWrapper[mat_idx].m_mat->m_ViewPos, scene->main_camera->m_Position.x, scene->main_camera->m_Position.y, scene->main_camera->m_Position.z));
			GLCall(glUniform3f(object->m_MaterialWrapper[mat_idx].m_mat->m_KsID, object->m_MaterialWrapper[mat_idx].m_mat->Ks.x, object->m_MaterialWrapper[mat_idx].m_mat->Ks.y, object->m_MaterialWrapper[mat_idx].m_mat->Ks.z));

			//texture
			if (object->m_MaterialWrapper[mat_idx].m_mat->m_Texture != NULL) {
				object->m_MaterialWrapper[mat_idx].m_mat->m_Texture->Bind();
			}
			else {
				//GLCall(glUniform3f(object->m_MaterialWrapper[i].m_mat->m_KaID, object->m_MaterialWrapper[i].m_mat->Ka.x, object->m_MaterialWrapper[i].m_mat->Ka.y, object->m_MaterialWrapper[i].m_mat->Ka.z));
				GLCall(glUniform3f(object->m_MaterialWrapper[mat_idx].m_mat->m_KdID, object->m_MaterialWrapper[mat_idx].m_mat->Kd.x, object->m_MaterialWrapper[mat_idx].m_mat->Kd.y, object->m_MaterialWrapper[mat_idx].m_mat->Kd.z));
			}


			//setup point lights
			for (unsigned int j = 0; j < MAX_POINT_LIGHTS; j++) {
				std::ostringstream strm;
				strm << "pointLights[" << j << "]";
				//strm << "pointLights";
				std::string light = strm.str();
				std::string pos_str = light + ".pos";
				std::string color_str = light + ".color";
				std::string a_str = light + ".a";
				std::string b_str = light + ".b";

				float myFloats[3] = { pointLights[j].pos.x, pointLights[j].pos.y, pointLights[j].pos.z };
				float myFloats2[3] = { pointLights[j].color.x, pointLights[j].color.y, pointLights[j].color.z };


				GLCall(glUniform3fv(object->m_MaterialWrapper[mat_idx].m_mat->m_Shader->GetUniformLocation(pos_str.c_str()),
					1,
					myFloats
				));

				GLCall(glUniform3fv(object->m_MaterialWrapper[mat_idx].m_mat->m_Shader->GetUniformLocation(color_str.c_str()),
					1,
					myFloats2
				));



				GLCall(glUniform1f(object->m_MaterialWrapper[mat_idx].m_mat->m_Shader->GetUniformLocation(a_str.c_str()),
					pointLights[j].a
				));

				GLCall(glUniform1f(object->m_MaterialWrapper[mat_idx].m_mat->m_Shader->GetUniformLocation(b_str.c_str()),
					pointLights[j].b
				));
			}
			
			GLCall(glDrawElements(GL_TRIANGLES, object->m_MaterialWrapper[mat_idx].m_Indices_Count, GL_UNSIGNED_INT, nullptr));
		}
    }
}

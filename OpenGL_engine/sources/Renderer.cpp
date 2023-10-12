#include "Renderer.h"

void Renderer::Render(Camera* cam)
{
    for(Object3D *object : m_Objects){
		object->m_va->Bind();
		for (unsigned int i = 0; i < object->m_MaterialWrapper.size(); i++) {
			object->m_MaterialWrapper[i].m_mat->Bind(); //set the material
			object->m_MaterialWrapper[i].m_ib->Bind(); //set the indexbuffer

			glm::mat4 model = glm::translate(glm::mat4(1.0f), object->m_Origin);
			model *= object->m_Model; //rotation

			// Send our transformation to the currently bound shader, in the "MVP" uniform
			// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
			GLCall(glUniformMatrix4fv(object->m_MaterialWrapper[i].m_mat->m_ViewMatrixID, 1, GL_FALSE, &cam->Get_View()[0][0]));
			GLCall(glUniformMatrix4fv(object->m_MaterialWrapper[i].m_mat->m_ProjMatrixID, 1, GL_FALSE, &cam->Get_Proj()[0][0]));
			GLCall(glUniformMatrix4fv(object->m_MaterialWrapper[i].m_mat->m_ModelMatrixID, 1, GL_FALSE, &model[0][0]));

			GLCall(glUniform3f(object->m_MaterialWrapper[i].m_mat->m_ViewPos, cam->m_Position.x, cam->m_Position.y, cam->m_Position.z));

			GLCall(glUniform3f(object->m_MaterialWrapper[i].m_mat->m_KaID, object->m_MaterialWrapper[i].m_mat->Ka.x, object->m_MaterialWrapper[i].m_mat->Ka.y, object->m_MaterialWrapper[i].m_mat->Ka.z));
			GLCall(glUniform3f(object->m_MaterialWrapper[i].m_mat->m_KdID, object->m_MaterialWrapper[i].m_mat->Kd.x, object->m_MaterialWrapper[i].m_mat->Kd.y, object->m_MaterialWrapper[i].m_mat->Kd.z));
			GLCall(glUniform3f(object->m_MaterialWrapper[i].m_mat->m_KsID, object->m_MaterialWrapper[i].m_mat->Ks.x, object->m_MaterialWrapper[i].m_mat->Ks.y, object->m_MaterialWrapper[i].m_mat->Ks.z));


			GLCall(glDrawElements(GL_TRIANGLES, object->m_MaterialWrapper[i].m_Indices_Count, GL_UNSIGNED_INT, nullptr));
		}
    }
}

void Renderer::AddObject3D(Object3D *obj)
{
    m_Objects.push_back(obj);
}

void Renderer::AddPointLight(PointLight *light)
{
    m_PointLights.push_back(light);
}

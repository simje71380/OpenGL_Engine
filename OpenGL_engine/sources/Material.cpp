#include "Material.h"

Material::Material(Shader* shader)
	: m_Shader(shader)
{
}

Material::Material(Shader* shader, bool is_light)
	: m_Shader(shader), m_IsLight(is_light)
{
}

void Material::Bind()
{
	//bind shader and texture
	m_Shader->Bind();
	m_ViewMatrixID = m_Shader->GetUniformLocation("View");
	m_ProjMatrixID = m_Shader->GetUniformLocation("Proj");
	m_ModelMatrixID = m_Shader->GetUniformLocation("Model");

	if (m_IsLight) return; //no need other params

	m_ViewPos = m_Shader->GetUniformLocation("ViewPos");
	m_KsID = m_Shader->GetUniformLocation("Ks");

	/*m_KsID = m_Shader->GetUniformLocation("MVP");
	m_KeID = m_Shader->GetUniformLocation("MVP");
	m_NsID = m_Shader->GetUniformLocation("MVP");
	m_NiID = m_Shader->GetUniformLocation("MVP");
	m_dID = m_Shader->GetUniformLocation("MVP");
	m_illumID = m_Shader->GetUniformLocation("MVP");*/


	if (m_Texture != NULL)
		m_Texture->Bind();
	else {
		//m_KaID = m_Shader->GetUniformLocation("Ka");
		m_KdID = m_Shader->GetUniformLocation("Kd");
	}

}

void Material::Unbind()
{
	m_Shader->Unbind();

	if (m_Texture != NULL)
		m_Texture->Unbind();
}

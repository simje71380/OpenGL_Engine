#include "Material.h"

Material::Material(Shader* shader)
	: m_Shader(shader)
{
}

void Material::Bind()
{
	//bind shader and texture
	m_Shader->Bind();
	m_MatrixID = m_Shader->GetUniformLocation("MVP");
	m_KaID = m_Shader->GetUniformLocation("Ka");
	/*m_KdID = m_Shader->GetUniformLocation("MVP");
	m_KsID = m_Shader->GetUniformLocation("MVP");
	m_KeID = m_Shader->GetUniformLocation("MVP");
	m_NsID = m_Shader->GetUniformLocation("MVP");
	m_NiID = m_Shader->GetUniformLocation("MVP");
	m_dID = m_Shader->GetUniformLocation("MVP");
	m_illumID = m_Shader->GetUniformLocation("MVP");*/


	if (m_Texture != NULL)
		m_Texture->Bind();

}

void Material::Unbind()
{
	m_Shader->Unbind();

	if (m_Texture != NULL)
		m_Texture->Unbind();
}

// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Skybox.cpp 
// Description : Skybox Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "Skybox.h"
#include "Statics.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "StaticMesh.h"

Skybox::Skybox()
{
	SetScale({1000,1000,1000});
	m_ShaderID = ShaderLoader::CreateShader("Fog_Skybox.vert","Fog_Skybox.frag");
}

Skybox::~Skybox()
{
}

void Skybox::SetCloudActive(bool _cloudActive)
{
	m_Cloud = _cloudActive;

	if (m_Cloud)
		InitCloud();
}

void Skybox::Draw()
{
	glUseProgram(m_ShaderID);

	// Pass In Cubemap Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture.ID);
	ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Texture0", 0);

	ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Foggy", Statics::Foggy);
	if (Statics::Foggy)
	{
		ShaderLoader::SetUniform1f(std::move(m_ShaderID), "FogStart", 5.0f);
		ShaderLoader::SetUniform1f(std::move(m_ShaderID), "FogDepth", 10.0f);
		ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "CameraPos", Statics::SceneCamera.GetPosition());
		ShaderLoader::SetUniform4fv(std::move(m_ShaderID), "FogColor", { 0.5f, 0.5f, 0.5f, 1.0f });
	}

	// Pass In PVM Matrix
	ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "Model", m_Transform.transform);
	ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "PVMMatrix", Statics::SceneCamera.GetPVMatrix() * m_Transform.transform);

	StaticMesh::InvertedCube->Draw();
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glUseProgram(0);
	
	// Draw Cloud If Enabled
	if (m_Cloud)
		DrawCloud();
}

void Skybox::SetTexture(Texture _cubemapTexture)
{
	m_CubemapTexture = _cubemapTexture;
}

Texture Skybox::GetTextureID()
{
	return m_CubemapTexture;
}

void Skybox::SetTranslation(glm::vec3 _newPosition)
{
	m_Transform.translation = _newPosition;
	UpdateModelValueOfTransform(m_Transform);
}

void Skybox::Translate(glm::vec3 _translation)
{
	m_Transform.translation += _translation;
	UpdateModelValueOfTransform(m_Transform);
}

void Skybox::SetRotation(glm::vec3 _axis, float _degrees)
{
	m_Transform.rotation_axis = _axis;
	m_Transform.rotation_value = glm::radians(_degrees);
	UpdateModelValueOfTransform(m_Transform);
}

void Skybox::Rotate(glm::vec3 _axis, float _degrees)
{
	m_Transform.rotation_axis = _axis;
	m_Transform.rotation_value += glm::radians(_degrees);
	UpdateModelValueOfTransform(m_Transform);
}

void Skybox::SetScale(glm::vec3 _newScale)
{
	m_Transform.scale = _newScale;
	UpdateModelValueOfTransform(m_Transform);
}

void Skybox::Scale(glm::vec3 _scaleFactor)
{
	m_Transform.scale *= _scaleFactor;
	UpdateModelValueOfTransform(m_Transform);
}

void Skybox::InitCloud()
{
	m_CloudShaderID = ShaderLoader::CreateShader("Fog.vert", "Fog.frag");
	m_CloudTexture = TextureLoader::LoadTexture("Heightmaps/RandomNoise.jpg");
}

void Skybox::DrawCloud()
{
	glUseProgram(m_CloudShaderID);
	Transform hemiSphereTransform = m_Transform;
	hemiSphereTransform.scale *= 0.8f;
	//hemiSphereTransform.scale = { 1,1,1 };
	hemiSphereTransform.translation.y = -hemiSphereTransform.scale.y / 4.0f;
	hemiSphereTransform.rotation_axis = { 1,0,0 };
	hemiSphereTransform.rotation_value = glm::radians(-90.0f);
	UpdateModelValueOfTransform(hemiSphereTransform);

	ShaderLoader::SetUniformMatrix4fv(std::move(m_CloudShaderID), "Model", hemiSphereTransform.transform);
	ShaderLoader::SetUniformMatrix4fv(std::move(m_CloudShaderID), "PVMatrix", Statics::SceneCamera.GetPVMatrix());
	ShaderLoader::SetUniformMatrix4fv(std::move(m_CloudShaderID), "PVMMatrix", Statics::SceneCamera.GetPVMatrix() * hemiSphereTransform.transform);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_CloudTexture.ID);
	ShaderLoader::SetUniform1i(std::move(m_CloudShaderID), "TextureCount", 1);
	ShaderLoader::SetUniform1i(std::move(m_CloudShaderID), "Texture0", 0);

	ShaderLoader::SetUniform1i(std::move(m_CloudShaderID), "Foggy", Statics::Foggy);
	if (Statics::Foggy)
	{
		ShaderLoader::SetUniform1f(std::move(m_CloudShaderID), "FogStart", 5.0f);
		ShaderLoader::SetUniform1f(std::move(m_CloudShaderID), "FogDepth", 10.0f);
		ShaderLoader::SetUniform3fv(std::move(m_CloudShaderID), "CameraPos", Statics::SceneCamera.GetPosition());
		ShaderLoader::SetUniform4fv(std::move(m_CloudShaderID), "FogColor", { 0.5f, 0.5f, 0.5f, 1.0f });
	}
	
	StaticMesh::Hemisphere->Draw();

	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

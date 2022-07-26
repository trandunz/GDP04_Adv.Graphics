// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : LightManager.cpp 
// Description : LightManager Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "LightManager.h"
#include "ShaderLoader.h"
#include "StaticMesh.h"
#include "Statics.h"

LightManager::LightManager(int _maxPointLights, int _maxDirectionalLights, int _maxSpotLights)
{
	m_MaxPointLights = _maxPointLights;
	m_MaxDirectionalLights = _maxDirectionalLights;
	m_MaxSpotLights = _maxSpotLights;
	m_UnlitMeshShaderID = ShaderLoader::CreateShader("SingleTexture.vert","UnlitColor.frag");
}

LightManager::~LightManager()
{
	m_PointLights.clear();
	m_DirectionalLights.clear();
	m_SpotLights.clear();
}

void LightManager::Draw()
{
	glUseProgram(m_UnlitMeshShaderID);
	for (auto& light : m_PointLights)
	{
		ShaderLoader::SetUniformMatrix4fv(std::move(m_UnlitMeshShaderID), "PVMMatrix", Statics::ActiveCamera.GetPVMatrix() * glm::translate(glm::mat4(1), light.Position));
		ShaderLoader::SetUniform4fv(std::move(m_UnlitMeshShaderID), "Color", light.Color);
		StaticMesh::Sphere->Draw();
	}
	glUseProgram(0);
}

void LightManager::CreatePointLight(PointLight _newLight)
{
	if ((int)m_PointLights.size() < m_MaxPointLights)
		m_PointLights.emplace_back(_newLight);
}

void LightManager::CreateDirectionalLight(DirectionalLight _newLight)
{
	if ((int)m_DirectionalLights.size() < m_MaxDirectionalLights)
		m_DirectionalLights.emplace_back(_newLight);
}

void LightManager::CreateSpotLight(SpotLight _newLight)
{
	if ((int)m_SpotLights.size() < m_MaxSpotLights)
		m_SpotLights.emplace_back(_newLight);
}

std::vector<PointLight>& LightManager::GetPointLights()
{
	return m_PointLights;
}

std::vector<DirectionalLight>& LightManager::GetDirectionalLights()
{
	return m_DirectionalLights;
}

std::vector<SpotLight>& LightManager::GetSpotLights()
{
	return m_SpotLights;
}

void LightManager::SetMaxPointLights(int _maxAmount)
{
	m_MaxPointLights = _maxAmount;

	// Trim Excess
	while ((int)m_PointLights.size() > m_MaxPointLights)
	{
		m_PointLights.pop_back();
	}
}

void LightManager::SetMaxDirectionalLights(int _maxAmount)
{
	m_MaxDirectionalLights = _maxAmount;

	// Trim Excess
	while ((int)m_DirectionalLights.size() > m_MaxDirectionalLights)
	{
		m_DirectionalLights.pop_back();
	}
}

void LightManager::SetMaxSpotLights(int _maxAmount)
{
	m_MaxSpotLights = _maxAmount;

	// Trim Excess
	while ((int)m_SpotLights.size() > m_MaxSpotLights)
	{
		m_SpotLights.pop_back();
	}
}

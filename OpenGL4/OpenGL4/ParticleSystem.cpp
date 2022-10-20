// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : ParticleSystem.cpp 
// Description : ParticleSystem Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "ParticleSystem.h"
#include "StaticMesh.h"
#include "ShaderLoader.h"

ParticleSystem::ParticleSystem(glm::vec3 _emissionPos, float _emissionRate)
{
	m_EmissionPosition = _emissionPos;
	m_EmissionRate = _emissionRate;
}

ParticleSystem::~ParticleSystem()
{
	m_Particles.clear();
}

void ParticleSystem::SetParticleTexture(Texture _texture)
{
	m_Texture = _texture;
}

void ParticleSystem::SetShader(std::string _vert, std::string _geo, std::string _frag)
{
	m_ShaderID = ShaderLoader::CreateShader(_vert, _geo, _frag);
}

void ParticleSystem::Pause()
{
	m_Paused = true;
}

void ParticleSystem::Stop()
{
	m_Paused = true;
	m_Particles.clear();
}

void ParticleSystem::Play()
{
	m_Paused = false;
}

void ParticleSystem::Update()
{
	if (!m_Paused)
	{
		m_EmissionTimer += Statics::DeltaTime;
		if (m_EmissionTimer >= m_EmissionRate)
		{
			m_EmissionTimer = 0.0f;
			m_Particles.emplace_back(Particle(m_EmissionPosition, { 0,-9.81,0.0f }, 2.0f));
			m_Particles.back().SetShader(m_ShaderID);
		}
	}

	for (auto& particle : m_Particles)
		particle.Update();
}

void ParticleSystem::Draw()
{
	for (auto& particle : m_Particles)
		particle.Draw();
}

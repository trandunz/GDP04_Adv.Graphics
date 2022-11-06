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
	m_EmissionTimer = m_EmissionRate;

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

void ParticleSystem::SetShader(std::string _vert, std::string _frag)
{
	m_ShaderID = ShaderLoader::CreateShader(_vert, _frag);
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
			if (!m_UseManualVelocity)
			{
				m_Velocity =
				{
					0.25f * cosf(m_Particles.size() * 0.1167f) + 0.95f * RandomFloat() - 0.25f,
					4.0f + 0.25f * RandomFloat() - 0.125f,
					0.25f * sinf(m_Particles.size() * 0.1167f) + 0.95f * RandomFloat() - 0.25f
				};
			}
			m_Particles.emplace_back(Particle(m_EmissionPosition, m_Velocity, m_Lifetime, m_Gravity));
			m_Particles.back().m_ColorOverLifetime = m_ColorOverLifetime;
		}
	}

	for (auto& particle : m_Particles)
		particle.Update();
}

void ParticleSystem::Draw()
{
	glUseProgram(m_ShaderID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	ShaderLoader::SetUniform1i(std::move(m_ShaderID), "TextureCount", 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture.ID);
	ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Texture0", 0);

	glm::vec3 vQuad1, vQuad2;
	glm::vec3 camFront = Statics::SceneCamera.GetFront();
	camFront = glm::normalize(camFront);
	vQuad1 = glm::cross(camFront, Statics::SceneCamera.GetUp());
	vQuad1 = glm::normalize(vQuad1);
	vQuad2 = glm::cross(camFront, vQuad1);
	vQuad2 = glm::normalize(vQuad2);
	ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "vQuad1", vQuad1);
	ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "vQuad2", vQuad2);

	for (auto& particle : m_Particles)
		particle.Draw(m_ShaderID);

	glDepthMask(GL_TRUE);

	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ParticleSystem::SetParticleVelocity(glm::vec3 _velocity)
{
	m_Velocity = _velocity;
	m_UseManualVelocity = true;
}

void ParticleSystem::SetGravity(bool _gravity)
{
	m_Gravity = _gravity;
	for (auto& particle : m_Particles)
	{
		particle.Gravity = _gravity;
	}
}

void ParticleSystem::SetLifetime(float _lifetime)
{
	m_Lifetime = _lifetime;
}

void ParticleSystem::SetAlphaOverLifetime(float _alpha)
{
	m_ColorOverLifetime.w = _alpha;
}

void ParticleSystem::Burst(int _count)
{
}

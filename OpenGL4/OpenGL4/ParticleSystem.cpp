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

	m_EmissionPosition = _emissionPos;
	m_Transform.translation = m_EmissionPosition;
	UpdateModelValueOfTransform(m_Transform);

	m_ShaderID = ShaderLoader::CreateShader("PointToQuad.vert", "PointToQuad.geo", "SingleTexture_Coloured.frag");
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

void ParticleSystem::Play()
{
	m_Paused = false;
}

void ParticleSystem::Update()
{
	if (!m_Paused)
	{
		m_EmissionTimer -= Statics::DeltaTime;

		if (m_Looping && m_EmissionTimer <= 0.0f)
			m_EmissionTimer = m_EmissionRate;

		for (int i = 0; i < m_Particles.size(); i++)
		{
			m_Particles[i].SetStartPosition(m_EmissionPosition);
			m_Particles[i].Update();
			m_Positions[i] = m_Particles[i].m_Transform.translation;
		}
	}
}

void ParticleSystem::Draw()
{
	if (m_EmissionTimer > 0)
	{
		glBindVertexArray(m_VertexArrayID);

		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * m_Positions.size(), &m_Positions[0]);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

		glUseProgram(m_ShaderID);

		ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "PVMMatrix", Statics::SceneCamera.GetPVMatrix());

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

		ShaderLoader::SetUniform4fv(std::move(m_ShaderID), "Color", glm::vec4(m_Color, 1.0f));
		//ShaderLoader::SetUniform1f(std::move(m_ShaderID), "AlphaOverLifetime", m_ColorOverLifetime.w);
		//ShaderLoader::SetUniform1f(std::move(m_ShaderID), "LifeTime", m_EmissionTimer / m_EmissionRate);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		glDrawArrays(GL_POINTS, 0, ParticleCount);

		glDepthMask(GL_TRUE);

		glDisableVertexAttribArray(0);
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}
}

void ParticleSystem::SetParticleVelocity(glm::vec3 _velocity)
{
	m_Velocity = _velocity;
	m_UseManualVelocity = true;
}

void ParticleSystem::SetParticleVelocity(std::function<glm::vec3()> _velocityFunction)
{
	m_VelocityFunction = _velocityFunction;
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

void ParticleSystem::SetLooping(bool _looping)
{
	m_Looping = _looping;
	for (auto& particle : m_Particles)
	{
		particle.Looping = _looping;
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
	if (m_EmissionTimer >= m_EmissionRate)
	{
		for (int i = 0; i < _count; i++)
		{
			if (!m_UseManualVelocity)
			{
				m_Velocity =
				{
					0.25f * cosf(m_Particles.size() * 0.1167f) + 0.95f * RandomFloat() - 0.25f,
					4.0f + 0.25f * RandomFloat() - 0.125f,
					0.25f * sinf(m_Particles.size() * 0.1167f) + 0.95f * RandomFloat() - 0.25f
				};
			} 
			if (m_VelocityFunction != nullptr)
			{
				m_Velocity = m_VelocityFunction();
			}
			m_Particles.emplace_back(Particle(m_EmissionPosition, m_Velocity, m_Lifetime, m_Gravity, m_Looping, m_Color));
			m_Particles.back().SetAlphaOverLifetime(m_ColorOverLifetime.w);
		}
	}
	else
	{
		m_EmissionTimer += Statics::DeltaTime;
	}
}

void ParticleSystem::SetColor(glm::vec3 _color)
{
	m_Color = _color;
	m_ColorOverLifetime = { m_Color.x, m_Color.y, m_Color.z, m_ColorOverLifetime.w };
	for (auto& particle : m_Particles)
	{
		particle.m_Color.x = _color.x;
		particle.m_Color.y = _color.y;
		particle.m_Color.z = _color.z;
	}
}

void ParticleSystem::Init()
{
	for (int i = 0; i < ParticleCount; i++)
	{
		if (!m_UseManualVelocity)
		{
			m_Velocity =
			{
				0.25f * cosf(m_Particles.size() * 0.1167f) + 0.95f * RandomFloat() - 0.25f,
				4.0f + 0.25f * RandomFloat() - 0.125f,
				0.25f * sinf(m_Particles.size() * 0.1167f) + 0.95f * RandomFloat() - 0.25f
			};
		}
		if (m_VelocityFunction != nullptr)
		{
			m_Velocity = m_VelocityFunction();
		}
		m_Particles.emplace_back(Particle(m_EmissionPosition, m_Velocity, m_Lifetime, m_Gravity, m_Looping, m_Color));
		m_Particles.back().SetAlphaOverLifetime(m_ColorOverLifetime.w);
		m_Positions.push_back(m_EmissionPosition);
		m_Particles.back().SetElaspedTime((RandomFloat() + 0.25f) * m_Lifetime);
	}

	// Vertex Array
	glGenVertexArrays(1, &m_VertexArrayID);
	glBindVertexArray(m_VertexArrayID);

	// Vertex Buffer
	glGenBuffers(1, &m_VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_Positions.size(), &m_Positions[0], GL_STATIC_DRAW);

	// Layouts
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

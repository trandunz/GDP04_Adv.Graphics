// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : ParticleSystem.cpp 
// Description : ParticleSystem Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "C_Particle_System.h"
#include "StaticMesh.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"

C_Particle_System::C_Particle_System(glm::vec3 _emissionPos, float _emissionRate)
{
	m_EmissionPosition = _emissionPos;
	m_Transform.translation = m_EmissionPosition;
	m_Transform.scale = { 0.01f,0.01f,0.01f };
	UpdateModelValueOfTransform(m_Transform);

	m_ShaderID = ShaderLoader::CreateShader("ParticleSystem.vert", "ParticleSystem.geo", "SingleTexture_Coloured.frag");
	m_ComputeID = ShaderLoader::CreateShader("ParticleSystem.comp");
	m_InitialPosition.resize(NUM_PARTICLES);
	m_InitialVelocity.resize(NUM_PARTICLES);
}

C_Particle_System::~C_Particle_System()
{
	m_InitialPosition.clear();
	m_InitialVelocity.clear();
}

void C_Particle_System::Init()
{
	for (int i = 0; i < NUM_PARTICLES; i++)
	{
		srand(i);
		m_InitialPosition[i] = glm::vec4(m_EmissionPosition + EmissionOffset, RandomFloat() * m_Lifetime);
		glm::vec3 velocity =
		{
			0.25f * cosf(m_InitialPosition.size() * 0.0167f) + 0.95f * RandomFloat() - (0.95f / 2.0f),
			YVelocity + 0.25f * RandomFloat() - 0.125f,
			0.25f * sinf(m_InitialPosition.size() * 0.0167f) + 0.95f * RandomFloat() - (0.95f / 2.0f)
		};
		m_InitialVelocity[i] = glm::vec4(velocity, RandomFloat() * m_Lifetime);
	}

	InitBuffers();
}

void C_Particle_System::SetParticleTexture(Texture _texture)
{
	m_Texture = _texture;
}

void C_Particle_System::SetGravity(float _strength)
{
	m_Gravity = _strength;
}

void C_Particle_System::Update()
{
}

void C_Particle_System::Draw()
{
	m_ElapsedTime += Statics::DeltaTime;

	// Compute
	glUseProgram(m_ComputeID);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_PositionBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_VelocityBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_InitialVelocityBuffer);

	ShaderLoader::SetUniform1f(std::move(m_ComputeID), "DeltaTime", Statics::DeltaTime);

	ShaderLoader::SetUniform1f(std::move(m_ComputeID), "Lifetime", m_Lifetime);

	ShaderLoader::SetUniform1f(std::move(m_ComputeID), "Gravity", m_Gravity);

	glDispatchCompute(NUM_PARTICLES / 128, 1, 1);

	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	// Render
	glUseProgram(m_ShaderID);

	glBindVertexArray(m_ParticleVertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, m_PositionBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, NULL, 0);

	ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "PVMMatrix", Statics::ActiveCamera->GetPVMatrix() * m_Transform.transform);

	ShaderLoader::SetUniform4fv(std::move(m_ShaderID), "ColorOverLifetime", { 1, 1, 1, 0 });

	ShaderLoader::SetUniform1i(std::move(m_ShaderID), "TextureCount", 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture.ID);
	ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Texture0", 0);

	glm::vec3 vQuad1, vQuad2;
	glm::vec3 camFront = Statics::ActiveCamera->GetFront();
	camFront = glm::normalize(camFront);
	vQuad1 = glm::cross(camFront, Statics::ActiveCamera->GetUp());
	vQuad1 = glm::normalize(vQuad1);
	vQuad2 = glm::cross(camFront, vQuad1);
	vQuad2 = glm::normalize(vQuad2);
	ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "vQuad1", vQuad1);
	ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "vQuad2", vQuad2);

	ShaderLoader::SetUniform4fv(std::move(m_ShaderID), "Color", { 1, 1, 1, 1 });

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);

	glDepthMask(GL_TRUE);

	glDisableVertexAttribArray(0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindVertexArray(0);
}

void C_Particle_System::InitBuffers()
{
	glGenBuffers(1, &m_PositionBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_PositionBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_InitialPosition.size() * sizeof(glm::vec4), m_InitialPosition.data(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_PositionBuffer);

	glGenBuffers(1, &m_VelocityBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_VelocityBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_InitialVelocity.size() * sizeof(glm::vec4), m_InitialVelocity.data(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_VelocityBuffer);

	glGenBuffers(1, &m_InitialVelocityBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_InitialVelocityBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_InitialVelocity.size() * sizeof(glm::vec4), m_InitialVelocity.data(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_InitialVelocityBuffer);

	glGenVertexArrays(1, &m_ParticleVertexArray);
	glBindVertexArray(m_ParticleVertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, m_PositionBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, NULL, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

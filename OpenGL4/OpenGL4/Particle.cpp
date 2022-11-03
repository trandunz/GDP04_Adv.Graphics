// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Particle.cpp 
// Description : Particle Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "Particle.h"
#include "StaticMesh.h"
#include "ShaderLoader.h"

Particle::Particle(glm::vec3 _pos, glm::vec3 _velocity, float _lifeTime, bool _gravity)
{
	m_StartPos = _pos;
	m_StartVelocity = _velocity;
	m_LifeTime = _lifeTime;
	Gravity = _gravity;

	ResetToInitialValues();
}

Particle::~Particle()
{
}

void Particle::Update()
{
	if (m_ElapsedTime <= 0)
	{
		ResetToInitialValues();
	}
	else
	{
		m_ElapsedTime -= Statics::DeltaTime;

		if (Gravity)
			m_Velocity += glm::vec3{0.0f, -9.81f, 0.0f} * Statics::DeltaTime;

		m_Transform.translation += m_Velocity * Statics::DeltaTime;
		m_Transform.scale = { 1.0f, 1.0f, 1.0f };
		UpdateModelValueOfTransform(m_Transform);
	}
}

void Particle::Draw(GLuint _shader)
{
	if (m_ElapsedTime > 0)
	{
		// Projection * View * Model Matrix
		ShaderLoader::SetUniformMatrix4fv(std::move(_shader), "PVMMatrix", Statics::SceneCamera.GetPVMatrix() * m_Transform.transform);

		m_Color = glm::mix({ 1,1,1,0 }, m_ColorOverLifetime, m_ElapsedTime / m_LifeTime);
		ShaderLoader::SetUniform4fv(std::move(_shader), "Color", m_Color);

		StaticMesh::Point->Draw();
	}
}

void Particle::ResetToInitialValues()
{
	m_Transform.translation = m_StartPos;
	m_Velocity = m_StartVelocity;
	m_ElapsedTime = m_LifeTime;
	UpdateModelValueOfTransform(m_Transform);
}

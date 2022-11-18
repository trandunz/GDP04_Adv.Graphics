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

Particle::Particle(glm::vec3 _pos, glm::vec3 _velocity, float _lifeTime)
{
	m_StartPos = _pos;
	m_StartVelocity = _velocity;
	m_LifeTime = _lifeTime;

	ResetToInitialValues();
}

Particle::~Particle()
{
}

void Particle::Update()
{
	if (m_ElapsedTime >= m_LifeTime)
	{
		ResetToInitialValues();
	}
	else
	{
		m_ElapsedTime += Statics::DeltaTime;
		Transform.translation += m_Velocity * Statics::DeltaTime;
	}
}

void Particle::Draw()
{
	if (m_ElapsedTime < m_LifeTime)
	{
		glUseProgram(m_ShaderID);

		StaticMesh::Point->Draw();

		glUseProgram(0);
	}
}

void Particle::SetShader(std::string _vert, std::string _geo, std::string _frag)
{
	m_ShaderID = ShaderLoader::CreateShader(_vert, _geo, _frag);
}

void Particle::SetShader(GLuint _shader)
{
	m_ShaderID = _shader;
}

void Particle::ResetToInitialValues()
{
	Transform.translation = m_StartPos;
	m_Velocity = m_StartVelocity;
	m_ElapsedTime = m_LifeTime;
}

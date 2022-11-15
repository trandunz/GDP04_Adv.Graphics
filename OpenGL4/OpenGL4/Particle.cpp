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

Particle::Particle(glm::vec3 _pos, glm::vec3 _velocity, float _lifeTime, bool _gravity, bool _looping, glm::vec3 _color)
{
	m_StartPos = _pos;
	m_StartVelocity = _velocity;
	m_Velocity = _velocity;
	m_LifeTime = _lifeTime;
	Looping = _looping;
	Gravity = _gravity;
	m_Color.x = _color.x;
	m_Color.y = _color.y;
	m_Color.z = _color.z;
	m_ColorOverLifetime = { m_Color.x, m_Color.y, m_Color.z, 1.0f };

	ResetToInitialValues();
}

Particle::~Particle()
{
}

void Particle::Update()
{
	if (m_ElapsedTime <= 0 && Looping)
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

void Particle::SetColorOverLifetime(glm::vec4 _color)
{
	m_ColorOverLifetime = _color;
}

void Particle::SetAlphaOverLifetime(float _alpha)
{
	m_ColorOverLifetime.w = _alpha;
}

void Particle::SetElaspedTime(float _elapsedTime)
{
	m_ElapsedTime = _elapsedTime;
}

void Particle::SetStartPosition(glm::vec3 _pos)
{
	m_StartPos = _pos;
}

void Particle::ResetToInitialValues()
{
	m_Transform.translation = m_StartPos;
	m_Velocity = m_StartVelocity;
	m_ElapsedTime = m_LifeTime;
	UpdateModelValueOfTransform(m_Transform);
}

// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Particle.h 
// Description : Particle Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "Statics.h"

class Particle
{
public:
	/// <summary>
	/// Particle constructor
	/// </summary>
	/// <param name="_pos"></param>
	/// <param name="_velocity"></param>
	/// <param name="_lifeTime"></param>
	Particle(glm::vec3 _pos, glm::vec3 _velocity, float _lifeTime, bool _gravity = true, bool _looping = true, glm::vec3 _color = {1,1,1});
	/// <summary>
	/// Particle Destructor
	/// </summary>
	~Particle();

	/// <summary>
	/// Particle update
	/// </summary>
	void Update();

	void SetColorOverLifetime(glm::vec4 _color);
	void SetAlphaOverLifetime(float _alpha);
	void SetElaspedTime(float _elapsedTime);
	void SetStartPosition(glm::vec3 _pos);

	bool Gravity{ true };
	bool Looping{ true };

	Transform m_Transform{};
	glm::vec4 m_Color{ 1,1,1,1 };
private:
	/// <summary>
	/// Reset the particles values back to the ones specified on construction
	/// </summary>
	void ResetToInitialValues();

	glm::vec4 m_ColorOverLifetime{ 1,1,1,1 };


	glm::vec3 m_Velocity{};
	float m_LifeTime{};

	glm::vec3 m_StartPos{};
	glm::vec3 m_StartVelocity{};
	float m_ElapsedTime{};
};


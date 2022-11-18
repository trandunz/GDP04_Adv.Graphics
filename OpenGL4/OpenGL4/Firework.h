// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Firework.h 
// Description : Firework Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "ParticleSystem.h"

class Firework
{
public:
	/// <summary>
	/// Firework constructor
	/// </summary>
	/// <param name="_startPos"></param>
	Firework(glm::vec3 _startPos = {});
	/// <summary>
	/// Firework destructor
	/// </summary>
	~Firework();

	/// <summary>
	/// Updates the firework
	/// </summary>
	void Update();
	/// <summary>
	/// Draws the firework
	/// </summary>
	void Draw();

	/// <summary>
	/// Resets the firework
	/// </summary>
	void Reset();
private:
	Transform m_Transform;
	glm::vec3 m_Colour{1,1,1};
	glm::vec3 m_InitialPosition{};
	float m_ExplosionHeight{ 10.0f };
	float m_MoveSpeed{ 10.0f };
	bool m_HasExploded{};
	bool m_Paused{true};
	ParticleSystem* m_TrailSystem{ nullptr };
	ParticleSystem* m_ExplosionSystem{ nullptr };
};


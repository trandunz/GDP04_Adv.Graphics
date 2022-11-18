// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : C_Particle_System.h 
// Description : C_Particle_System Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "Particle.h"

#define NUM_PARTICLES 128 * 1000

class C_Particle_System
{
public:
	/// <summary>
	/// Particle system constructor
	/// </summary>
	/// <param name="_emissionPos"></param>
	/// <param name="_emissionRate"></param>
	C_Particle_System(glm::vec3 _emissionPos, float _emissionRate);
	/// <summary>
	/// Particle system destructor
	/// </summary>
	~C_Particle_System();

	/// <summary>
	/// Initializes the particle system
	/// </summary>
	void Init();

	/// <summary>
	/// Set the particles texture
	/// </summary>
	/// <param name="_texture"></param>
	void SetParticleTexture(Texture _texture);

	/// <summary>
	/// Sets the strength of gravity
	/// </summary>
	/// <param name="_strength"></param>
	void SetGravity(float _strength);

	/// <summary>
	/// Updates all the particles
	/// </summary>
	void Update();
	/// <summary>
	/// Draw all the particles
	/// </summary>
	void Draw();

	/// <summary>
	/// Initializes the buffers
	/// </summary>
	void InitBuffers();

	/// <summary>
	/// Sets the offset function
	/// </summary>
	/// <param name="_offsetFunction"></param>
	void SetParticleOffset(std::function<glm::vec3()> _offsetFunction);

	std::function<glm::vec3()> EmissionOffset{nullptr};
	float YVelocity{ 2.0f };
	float Lifetime{ 1.0f };
private:
	Texture m_Texture{};
	Transform m_Transform{};
	std::vector<glm::vec4> m_InitialPosition{};
	std::vector<glm::vec4> m_InitialVelocity{};
	std::vector<float> m_InitialRotation{};
	glm::vec3 m_EmissionPosition{};

	bool m_Paused{};
	float m_EmissionRate{};
	float m_EmissionTimer{};
	float m_ElapsedTime{};
	float m_Gravity{ 9.81f };

	GLuint m_ShaderID{};
	GLuint m_ComputeID{};
	GLuint m_PositionBuffer{};
	GLuint m_RotationBuffer{};
	GLuint m_VelocityBuffer{};
	GLuint m_InitialVelocityBuffer{};
	GLuint m_InitialPositionBuffer{};
	GLuint m_ParticleVertexArray{};
};


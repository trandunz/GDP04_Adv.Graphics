#pragma once
#include "Particle.h"

#define NUM_PARTICLES 128 * 100

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

	void Init();

	/// <summary>
	/// Set the particles texture
	/// </summary>
	/// <param name="_texture"></param>
	void SetParticleTexture(Texture _texture);

	void SetGravity(float _strength);

	/// <summary>
	/// Updates all the particles
	/// </summary>
	void Update();
	/// <summary>
	/// Draw all the particles
	/// </summary>
	void Draw();

	void InitBuffers();

	glm::vec3 EmissionOffset{};
	float YVelocity{ 2.0f };
	float m_Lifetime{ 1.0f };
private:
	Texture m_Texture{};
	Transform m_Transform{};
	std::vector<glm::vec4> m_InitialPosition{};
	std::vector<glm::vec4> m_InitialVelocity{};
	glm::vec3 m_EmissionPosition{};

	bool m_Paused{};
	float m_EmissionRate{};
	float m_EmissionTimer{};
	float m_ElapsedTime{};
	float m_Gravity{ 9.81f };

	GLuint m_ShaderID{};
	GLuint m_ComputeID{};
	GLuint m_PositionBuffer{};
	GLuint m_VelocityBuffer{};
	GLuint m_InitialVelocityBuffer{};
	GLuint m_ParticleVertexArray{};
};


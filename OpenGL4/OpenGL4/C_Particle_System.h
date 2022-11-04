#pragma once
#include "Particle.h"

#define NUM_PARTICLES 128 * 2000

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
	/// Set the particles texture
	/// </summary>
	/// <param name="_texture"></param>
	void SetParticleTexture(Texture _texture);

	/// <summary>
	/// Updates all the particles
	/// </summary>
	void Update();
	/// <summary>
	/// Draw all the particles
	/// </summary>
	void Draw();

	void InitBuffers();
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

	GLuint m_ShaderID{};
	GLuint m_ComputeID{};
	GLuint m_PositionBuffer{};
	GLuint m_VelocityBuffer{};
	GLuint m_InitialVelocityBuffer{};
	GLuint m_ParticleVertexArray{};
};


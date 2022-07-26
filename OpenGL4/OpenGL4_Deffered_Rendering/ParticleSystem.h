// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : ParticleSystem.h 
// Description : ParticleSystem Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "Particle.h"

class ParticleSystem
{
public:
	/// <summary>
	/// Particle system constructor
	/// </summary>
	/// <param name="_emissionPos"></param>
	/// <param name="_emissionRate"></param>
	ParticleSystem(glm::vec3 _emissionPos, float _emissionRate);
	/// <summary>
	/// Particle system destructor
	/// </summary>
	~ParticleSystem();

	/// <summary>
	/// Set the particles texture
	/// </summary>
	/// <param name="_texture"></param>
	void SetParticleTexture(Texture _texture);
	/// <summary>
	/// Set the shader of the particles
	/// </summary>
	/// <param name="_vert"></param>
	/// <param name="_geo"></param>
	/// <param name="_frag"></param>
	void SetShader(std::string _vert, std::string _geo, std::string _frag);
	
	/// <summary>
	/// Pause the particle system
	/// </summary>
	void Pause();
	/// <summary>
	/// Stop the particle system
	/// </summary>
	void Stop();
	/// <summary>
	/// Play the particle systems
	/// </summary>
	void Play();

	/// <summary>
	/// Updates all the particles
	/// </summary>
	void Update();
	/// <summary>
	/// Draw all the particles
	/// </summary>
	void Draw();

private:
	Texture m_Texture{};
	std::vector<Particle> m_Particles{};
	glm::vec3 EmissionPosition{};
	float m_EmissionRate{};
	float m_EmissionTimer{};

	bool m_Paused{ false };

	GLuint m_ShaderID{};
};


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
	/// Set the shader of the particles
	/// </summary>
	/// <param name="_vert"></param>
	/// <param name="_frag"></param>
	void SetShader(std::string _vert, std::string _frag);
	
	/// <summary>
	/// Pause the particle system
	/// </summary>
	void Pause();

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

	/// <summary>
	/// Sets the particle velocity
	/// </summary>
	/// <param name="_velocity"></param>
	void SetParticleVelocity(glm::vec3 _velocity);
	
	/// <summary>
	/// Sets the particle velocity function
	/// </summary>
	/// <param name="_velocityFunction"></param>
	void SetParticleVelocity(std::function<glm::vec3()> _velocityFunction);

	/// <summary>
	/// sets the particle gravity strength
	/// </summary>
	/// <param name="_gravity"></param>
	void SetGravity(bool _gravity);

	/// <summary>
	/// Sets if its looping
	/// </summary>
	/// <param name="_looping"></param>
	void SetLooping(bool _looping);

	/// <summary>
	/// Sets particle lifetime
	/// </summary>
	/// <param name="_lifetime"></param>
	void SetLifetime(float _lifetime);

	/// <summary>
	/// Sets alpha over lifetime
	/// </summary>
	/// <param name="_alpha"></param>
	void SetAlphaOverLifetime(float _alpha);

	/// <summary>
	/// Creates a particle burst of the specified amount
	/// </summary>
	/// <param name="_count"></param>
	void Burst(int _count);

	/// <summary>
	/// Sets trhe color of the particles
	/// </summary>
	/// <param name="_color"></param>
	void SetColor(glm::vec3 _color);

	/// <summary>
	/// Initializes the particle system
	/// </summary>
	void Init();

	/// <summary>
	/// Resets the particle system
	/// </summary>
	void ResetParticles();
	
	int ParticleCount{128 * 100};
	glm::vec3 EmissionPosition{};
private:
	GLuint m_ShaderID{};

	GLuint m_VertexArrayID{};
	GLuint m_VertexBufferID{};

	Transform m_Transform{};
	Texture m_Texture{};
	std::vector<Particle> m_Particles{};
	std::vector<glm::vec3> m_Positions{};

	std::function<glm::vec3()> m_VelocityFunction{ nullptr };
	bool m_UseManualVelocity{false};
	glm::vec3 m_Velocity{};
	float m_EmissionRate{};
	float m_EmissionTimer{};
	float m_Lifetime{0.7f};
	glm::vec4 m_ColorOverLifetime{1,1,1,1};
	glm::vec3 m_Color{ 1,1,1 };

	bool m_Gravity{ true };
	bool m_Paused{ false };
	bool m_Looping{ true };
};


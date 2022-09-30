#pragma once
#include "Particle.h"

class ParticleSystem
{
public:
	ParticleSystem(glm::vec3 _emissionPos, float _emissionRate);
	~ParticleSystem();

	void SetParticleTexture(Texture _texture);
	void SetShader(std::string _vert, std::string _geo, std::string _frag);

	void Pause();
	void Stop();
	void Play();

	void Update();
	void Draw();

private:
	Texture m_Texture{};
	std::vector<Particle> m_Particles{};
	glm::vec3 m_EmissionPosition{};
	float m_EmissionRate{};
	float m_EmissionTimer{};

	bool m_Paused{ false };

	GLuint m_ShaderID{};
};


#pragma once
#include "Statics.h"

class Particle
{
public:
	Particle(glm::vec3 _pos, glm::vec3 _velocity, float _lifeTime);
	~Particle();

	void Update();
	void Draw();

	void SetShader(std::string _vert, std::string _geo, std::string _frag);
	void SetShader(GLuint _shader);

private:
	void ResetToInitialValues();

	Transform m_Transform{};
	glm::vec3 m_Velocity{};
	float m_LifeTime{};

	glm::vec3 m_StartPos{};
	glm::vec3 m_StartVelocity{};
	float m_ElapsedTime{};

	GLuint m_ShaderID{};
};


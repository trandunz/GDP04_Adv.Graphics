#pragma once
#include "ParticleSystem.h"

class Firework
{
public:
	Firework(glm::vec3 _startPos = {});
	~Firework();

	void Update();
	void Draw();

private:
	Transform m_Transform;
	glm::vec3 m_Colour{1,1,1};
	glm::vec3 m_InitialPosition{};
	float m_ExplosionHeight{ 10.0f };
	float m_MoveSpeed{ 10.0f };
	bool m_HasExploded{};
	ParticleSystem* m_TrailSystem{ nullptr };
	ParticleSystem* m_ExplosionSystem{ nullptr };
};


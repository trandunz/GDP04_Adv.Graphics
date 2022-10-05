#pragma once
#include "GameObject.h"

class ClothParticle : public GameObject
{
public:
	ClothParticle(glm::vec3 _startPos = {0,0,-10});
	~ClothParticle();

	void Update() override;

	void Move(glm::vec3 _amount, bool _useDt = true);

private:
	glm::vec3 m_Position{};
	glm::vec3 m_PreviousPosition{};

public:
	glm::vec3 GetPosition() const;
};

class Cloth
{
public:
	Cloth();
	~Cloth();

	void Update();
	void Draw();

private:
	std::vector<ClothParticle*> m_Particles{};

	glm::vec3 m_Position{0,0,-10};
};


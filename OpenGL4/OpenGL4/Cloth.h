#pragma once
#include "GameObject.h"

class DistanceJoint;
class ClothParticle : public GameObject
{
public:
	ClothParticle(glm::vec3 _startPos = {0,0,-10});
	~ClothParticle();

	void Update() override;

	void Move(glm::vec3 _amount, bool _useDt = true);

	void TogglePinned();
private:
	bool m_IsPinned{ false };
	glm::vec3 m_Position{};
	glm::vec3 m_PreviousPosition{};
	glm::vec3 m_StartPosition{};
public:
	glm::vec3 GetPosition() const;
};

class Cloth
{
public:
	Cloth(unsigned width, unsigned height, unsigned spacing, glm::vec3 _startPos);
	~Cloth();

	void Update();
	void Draw();

private:
	std::vector<ClothParticle*> m_Particles{};
	std::vector< DistanceJoint*> m_DistanceJoints{};

	glm::vec3 m_Position{0,01,-10};
};


#pragma once
class ClothParticle;
class DistanceJoint
{
public:
	DistanceJoint(ClothParticle* _p1, ClothParticle* _p2, float _length);
	~DistanceJoint();

	void Update();

private:
	float m_Length{};
	ClothParticle* m_P1{};
	ClothParticle* m_P2{};
};


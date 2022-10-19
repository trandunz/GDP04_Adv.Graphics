#pragma once
class ClothParticle;
class DistanceJoint
{
public:
	DistanceJoint(ClothParticle* _p1, ClothParticle* _p2, float _length);
	~DistanceJoint();

	void Update();

	ClothParticle* GetP1();
	ClothParticle* GetP2();

	float m_Stiffness{ 500.0f };
	float m_Length{};
private:

	ClothParticle* m_P1{};
	ClothParticle* m_P2{};
};


// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : DistanceJoint.h 
// Description : DistanceJoint Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
class ClothParticle;
class DistanceJoint
{
public:
	/// <summary>
	/// Distance joint constructor
	/// </summary>
	/// <param name="_p1"></param>
	/// <param name="_p2"></param>
	/// <param name="_length"></param>
	DistanceJoint(ClothParticle* _p1, ClothParticle* _p2, float _length);
	/// <summary>
	/// Distance joint destructor
	/// </summary>
	~DistanceJoint();

	/// <summary>
	/// Update the distance joint and apply a force too the connected particles
	/// </summary>
	void Update();

	ClothParticle* GetP1();
	ClothParticle* GetP2();

	float m_Stiffness{ 500.0f };
	float m_Length{};
	float m_MaxForce{ 100.0f };
	bool Destroy{};
private:

	ClothParticle* m_P1{};
	ClothParticle* m_P2{};
};


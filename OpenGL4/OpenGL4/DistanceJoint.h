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
#include "Statics.h"
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

	void Draw(glm::vec3 _colour = {1,1,1});

	ClothParticle* GetP1();
	ClothParticle* GetP2();

	float m_Stiffness{ 500.0f };
	float m_Length{};
	float m_MaxForce{ 175.0f };
	bool Destroy{};
private:
	GLuint m_ShaderID{};
	ClothParticle* m_P1{ nullptr };
	ClothParticle* m_P2{ nullptr };
};


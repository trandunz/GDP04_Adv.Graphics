// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : DistanceJoint.cpp 
// Description : DistanceJoint Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "DistanceJoint.h"
#include "Cloth.h"
#include "Helper.h"

DistanceJoint::DistanceJoint(ClothParticle* _p1, ClothParticle* _p2, float _length)
{
	m_P1 = _p1;
	m_P2 = _p2;
	m_P1->ConstraintLength = _length;
	m_P2->ConstraintLength = _length;
	Length = _length;
}

DistanceJoint::~DistanceJoint()
{
	m_P1 = nullptr;
	m_P2 = nullptr;
}

void DistanceJoint::Update()
{
	glm::vec3 delta = m_P1->GetPosition() - m_P2->GetPosition();
	float deltaLength = glm::length(delta);
	float difference = (Length - deltaLength) / deltaLength;
	auto im1 = 1 / m_P1->GetMass();
	auto im2 = 1 / m_P2->GetMass();
	auto force1 = delta * (im1 / (im1 + im2)) * Stiffness * difference;
	auto force2 = -delta * (im2 / (im1 + im2)) * Stiffness * difference;

    m_P1->ApplyForce(force1);
	m_P2->ApplyForce(force2);
}

ClothParticle* DistanceJoint::GetP1()
{
	return m_P1;
}

ClothParticle* DistanceJoint::GetP2()
{
	return m_P2;
}

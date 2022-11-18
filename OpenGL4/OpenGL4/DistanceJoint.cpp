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
#include "ShaderLoader.h"
#include "StaticMesh.h"

DistanceJoint::DistanceJoint(ClothParticle* _p1, ClothParticle* _p2, float _length)
{
	m_P1 = _p1;
	m_P2 = _p2;
	m_P1->ConstraintLength = _length;
	m_P2->ConstraintLength = _length;
	Length = _length;

	m_ShaderID = ShaderLoader::CreateShader("Normals3D.vert", "DynamicLine.geo", "UnlitColor.frag");
}

DistanceJoint::~DistanceJoint()
{
	m_P1 = nullptr;
	m_P2 = nullptr;
}

void DistanceJoint::Update()
{
	if (m_P1 && m_P2)
	{
		// get thee vector bwteeen the two particles
		glm::vec3 delta = m_P1->GetPosition() - m_P2->GetPosition();
		// get the length of the vector
		float deltaLength = glm::length(delta);
		// get the difference between deltaLength and the resting distance
		float difference = (Length - deltaLength) / deltaLength;
		auto im1 = 1 / m_P1->GetMass();
		auto im2 = 1 / m_P2->GetMass();
		// calculate a force for the first particle
		auto force1 = delta * (im1 / (im1 + im2)) * Stiffness * difference;
		// calculate a force for the second particle in the oppiosite direction
		auto force2 = -delta * (im2 / (im1 + im2)) * Stiffness * difference;

		// apply the forces
		m_P1->ApplyForce(force1);
		m_P2->ApplyForce(force2);

		// of the force was too large then break the joint
		if (glm::length(force1) >= MaxForce)
		{
			Destroy = true;
		}
	}
}

void DistanceJoint::Draw(glm::vec3 _colour)
{
	glUseProgram(m_ShaderID);
	ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "PVMatrix", Statics::ActiveCamera->GetPVMatrix());
	ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[0]", m_P1->GetPosition());
	ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[1]", m_P2->GetPosition());
	ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[1]", m_P2->GetPosition());
	ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Color", _colour);
	StaticMesh::Line->Draw();
	glUseProgram(0);
}

ClothParticle* DistanceJoint::GetP1()
{
	return m_P1;
}

ClothParticle* DistanceJoint::GetP2()
{
	return m_P2;
}

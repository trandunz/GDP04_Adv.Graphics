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
	m_P1->m_ConstraintLength = _length;
	m_P2->m_ConstraintLength = _length;
	m_Length = _length;

	m_ShaderID = ShaderLoader::CreateShader("Normals3D.vert", "DynamicLine.geo", "SingleTexture.frag");
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
		glm::vec3 delta = m_P1->GetPosition() - m_P2->GetPosition();
		float deltaLength = glm::length(delta);
		float difference = (m_Length - deltaLength) / deltaLength;
		auto im1 = 1 / m_P1->GetMass();
		auto im2 = 1 / m_P2->GetMass();
		auto force1 = delta * (im1 / (im1 + im2)) * m_Stiffness * difference;
		auto force2 = -delta * (im2 / (im1 + im2)) * m_Stiffness * difference;

		m_P1->ApplyForce(force1);
		m_P2->ApplyForce(force2);

		if (glm::length(force1) >= m_MaxForce)
		{
			Destroy = true;
		}
	}
}

void DistanceJoint::Draw()
{
	glUseProgram(m_ShaderID);
	ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "PVMatrix", Statics::SceneCamera.GetPVMatrix()); 
	ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[0]", m_P1->GetPosition());
	ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[1]", m_P2->GetPosition());
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

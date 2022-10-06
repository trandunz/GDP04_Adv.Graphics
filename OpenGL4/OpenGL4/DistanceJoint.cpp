#include "DistanceJoint.h"
#include "Cloth.h"
#include "Helper.h"

DistanceJoint::DistanceJoint(ClothParticle* _p1, ClothParticle* _p2, float _length)
{
	m_P1 = _p1;
	m_P2 = _p2;
	m_Length = _length;
}

DistanceJoint::~DistanceJoint()
{
	m_P1 = nullptr;
	m_P2 = nullptr;
}

void DistanceJoint::Update()
{
    auto diff = m_P1->GetPosition() - m_P2->GetPosition();
    auto diffFactor = (m_Length - Magnitude(diff)) / Magnitude(diff) * 0.5f;
	auto offset = diff * diffFactor;

    m_P1->Move(offset, false);
	m_P2->Move(-offset, false);
}

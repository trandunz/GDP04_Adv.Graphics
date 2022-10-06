#include "Cloth.h"
#include "DistanceJoint.h"

Cloth::Cloth(unsigned width, unsigned height, float spacing, glm::vec3 _startPos)
{
	for (int y = 0; y <= height; y++) 
	{
		for (int x = 0; x <= width; x++) 
		{
			auto particle = new ClothParticle({ _startPos.x + (x * (float)spacing),_startPos.y - (y * (float)spacing),_startPos.z });

			if (x != 0) 
			{
				m_DistanceJoints.emplace_back(new DistanceJoint(particle, m_Particles.back(), spacing));
			}

			if (y != 0) 
			{
				m_DistanceJoints.emplace_back(new DistanceJoint(particle, m_Particles[x + (y - 1) * (width + 1)], spacing));
			}

			if (y == 0) {
				particle->TogglePinned();
			}

			m_Particles.emplace_back(particle);
		}
	}
}

Cloth::~Cloth()
{
	for (auto& particle : m_Particles)
	{
		if (particle)
			delete particle;
		particle = nullptr;
	}
	m_Particles.clear();

	for (auto& joint : m_DistanceJoints)
	{
		if (joint)
			delete joint;
		joint = nullptr;
	}
	m_DistanceJoints.clear();
}

void Cloth::Update()
{
	for (auto& particle : m_Particles)
	{
		particle->Update();
	}
	for (auto& distanceJoint : m_DistanceJoints)
	{
		distanceJoint->Update();
	}
}

void Cloth::Draw()
{
	for (auto& particle : m_Particles)
	{
		particle->Draw();
	}
}

ClothParticle::ClothParticle(glm::vec3 _startPos)
{
	m_Position = _startPos;
	m_PreviousPosition = _startPos;
	m_StartPosition = _startPos;
	SetMesh(StaticMesh::Sphere);
	SetShader("PositionOnly.vert", "UnlitColor.frag");
	SetTranslation(_startPos);
	SetScale({ 0.1f,0.1f,0.1f });
}

ClothParticle::~ClothParticle()
{
}

void ClothParticle::Update()
{
	if (m_IsPinned)
	{
		SetTranslation(m_StartPosition);
		return;
	}

	glm::vec3 force = { 0.1f,-1.0f, -0.5f };

	auto acceleration = force;

	auto prevPosition = m_Position;

	m_Position = m_Position * 2.0f - m_PreviousPosition + acceleration * (1/60.0f * 1 / 60.0f);

	m_PreviousPosition = prevPosition;
	
	SetTranslation(m_Position);
}

void ClothParticle::Move(glm::vec3 _amount, bool _useDt)
{
	if (!m_IsPinned)
	{
		if (_useDt)
			m_Position += _amount * (1 / 60.0f);
		else
			m_Position += _amount;
	}
}

void ClothParticle::TogglePinned()
{
	m_IsPinned = !m_IsPinned;
}

glm::vec3 ClothParticle::GetPosition() const
{
	return m_Position;
}

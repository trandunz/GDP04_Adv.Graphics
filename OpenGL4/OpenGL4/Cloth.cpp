#include "Cloth.h"

Cloth::Cloth()
{
	m_Particles.emplace_back(new ClothParticle(m_Position));
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
}

void Cloth::Update()
{
	for (auto& particle : m_Particles)
	{
		particle->Update();
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
	glm::vec3 force = { 0,-0.1f, 0 };

	auto acceleration = force;

	auto prevPosition = m_Position;

	m_Position = m_Position * 2.0f - m_PreviousPosition + acceleration * (Statics::DeltaTime * Statics::DeltaTime);

	m_PreviousPosition = prevPosition;

	SetTranslation(m_Position);
}

void ClothParticle::Move(glm::vec3 _amount, bool _useDt)
{
	if (_useDt)
		m_Position += _amount * Statics::DeltaTime;
	else
		m_Position += _amount;
}

glm::vec3 ClothParticle::GetPosition() const
{
	return m_Position;
}

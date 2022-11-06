#include "Firework.h"
#include "TextureLoader.h"

Firework::Firework(glm::vec3 _startPos)
{
	m_InitialPosition = _startPos;
	m_Transform.translation = _startPos;
	UpdateModelValueOfTransform(m_Transform);

	m_TrailSystem = new ParticleSystem(_startPos, 0.1f);
	m_TrailSystem->SetShader("PointToQuad.vert", "PointToQuad.geo", "SingleTexture_Coloured.frag");
	m_TrailSystem->SetParticleTexture(TextureLoader::LoadTexture("Smoke.png"));
	m_TrailSystem->SetGravity(false);
	m_TrailSystem->SetParticleVelocity({});
	m_TrailSystem->SetLifetime(3.0f);
	m_TrailSystem->SetAlphaOverLifetime(1);

	m_ExplosionSystem = new ParticleSystem(_startPos, 0.1f);
	m_ExplosionSystem->SetShader("PointToQuad.vert", "PointToQuad.geo", "SingleTexture_Coloured.frag");
	m_ExplosionSystem->SetParticleTexture(TextureLoader::LoadTexture("Flame.png"));
	m_ExplosionSystem->SetGravity(false);
	m_ExplosionSystem->SetParticleVelocity({(rand() % 10) - 5, 0, (rand() % 10) - 5 });
	m_ExplosionSystem->SetLifetime(2.0f);
	m_ExplosionSystem->SetAlphaOverLifetime(1);
}

Firework::~Firework()
{
	if (m_TrailSystem)
		delete m_TrailSystem;
	m_TrailSystem = nullptr;

	if (m_ExplosionSystem)
		delete m_ExplosionSystem;
	m_ExplosionSystem = nullptr;
}

void Firework::Update()
{
	if (m_Transform.translation.y < m_InitialPosition.y + m_ExplosionHeight)
	{
		m_Transform.translation.y += Statics::DeltaTime * m_MoveSpeed;
		UpdateModelValueOfTransform(m_Transform);

		m_ExplosionSystem->Stop();
		m_TrailSystem->Play();
	}
	else
	{
		m_TrailSystem->Stop();
		m_ExplosionSystem->Play();
	}

	m_TrailSystem->m_EmissionPosition = m_Transform.translation;
	m_ExplosionSystem->m_EmissionPosition = m_Transform.translation;
	m_ExplosionSystem->SetParticleVelocity({ (rand() % 10) - 5, 0, (rand() % 10) - 5 });
	m_TrailSystem->Update();
	m_ExplosionSystem->Update();
}

void Firework::Draw()
{
	m_TrailSystem->Draw();
	m_ExplosionSystem->Draw();
}

#include "Firework.h"
#include "TextureLoader.h"

Firework::Firework(glm::vec3 _startPos)
{
	m_InitialPosition = _startPos;
	m_Transform.translation = _startPos;
	UpdateModelValueOfTransform(m_Transform);

	m_TrailSystem = new ParticleSystem(_startPos, 0.1f);
	m_TrailSystem->SetParticleTexture(TextureLoader::LoadTexture("Smoke.png"));
	m_TrailSystem->SetGravity(false);
	m_TrailSystem->SetParticleVelocity(glm::vec3());
	m_TrailSystem->SetLifetime(3.0f);
	m_TrailSystem->SetAlphaOverLifetime(1);
	m_TrailSystem->Init();

	m_ExplosionSystem = new ParticleSystem(_startPos, 0.1f);
	m_ExplosionSystem->SetParticleTexture(TextureLoader::LoadTexture("Smoke.png"));
	m_ExplosionSystem->SetGravity(false);
	m_ExplosionSystem->SetParticleVelocity([]()->glm::vec3
		{
			return glm::vec3{ (rand() % 10) - 5, (rand() % 10) - 5, (rand() % 10) - 5 };
		});
	m_ExplosionSystem->SetLifetime(2.0f);
	m_ExplosionSystem->SetLooping(false);
	m_ExplosionSystem->SetAlphaOverLifetime(1);
	m_ExplosionSystem->SetColor({ (rand() % 10)/10.0f ,(rand() % 10) / 10.0f ,(rand() % 10) / 10.0f });
	m_ExplosionSystem->Init();
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

		m_ExplosionSystem->Pause();
		m_TrailSystem->Play();
	}
	else if (!m_HasExploded)
	{
		m_HasExploded = true;
		m_TrailSystem->Stop();
		m_ExplosionSystem->Burst(10);
	}

	m_TrailSystem->m_EmissionPosition = m_Transform.translation;
	m_ExplosionSystem->m_EmissionPosition = m_Transform.translation;
	m_TrailSystem->Update();
	m_ExplosionSystem->Update();
}

void Firework::Draw()
{
	m_TrailSystem->Draw();
	m_ExplosionSystem->Draw();
}
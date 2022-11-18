#include "Firework.h"
#include "TextureLoader.h"

Firework::Firework(glm::vec3 _startPos)
{
	m_InitialPosition = _startPos;
	m_Transform.translation = _startPos;
	UpdateModelValueOfTransform(m_Transform);

	m_TrailSystem = new ParticleSystem(_startPos, 0.3f);
	m_TrailSystem->SetParticleTexture(TextureLoader::LoadTexture("Smoke.png"));
	m_TrailSystem->SetGravity(false);
	m_TrailSystem->SetParticleVelocity(glm::vec3());
	m_TrailSystem->SetAlphaOverLifetime(1);
	m_TrailSystem->Init();
	m_TrailSystem->Pause();

	m_ExplosionSystem = new ParticleSystem(_startPos, 1.0f);
	m_ExplosionSystem->SetParticleTexture(TextureLoader::LoadTexture("Smoke.png"));
	m_ExplosionSystem->SetGravity(true);
	m_ExplosionSystem->SetParticleVelocity([]()->glm::vec3
		{
			return glm::vec3{ (rand() % 10) - 5, 10, (rand() % 10) - 5 };
		});
	m_ExplosionSystem->SetLifetime(1.0f);
	m_ExplosionSystem->SetAlphaOverLifetime(1);
	m_ExplosionSystem->SetColor({ (rand() % 10) / 10.0f ,(rand() % 10) / 10.0f ,(rand() % 10) / 10.0f });
	m_ExplosionSystem->SetLooping(false);
	m_ExplosionSystem->ParticleCount = 10;
	m_ExplosionSystem->Init();
	m_ExplosionSystem->Pause();

	m_Paused = true;
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
	if (!m_Paused)
	{
		m_TrailSystem->m_EmissionPosition = m_Transform.translation;
		m_ExplosionSystem->m_EmissionPosition = m_Transform.translation;

		if (m_Transform.translation.y < m_InitialPosition.y + m_ExplosionHeight)
		{
			m_Transform.translation.y += Statics::DeltaTime * m_MoveSpeed;
			UpdateModelValueOfTransform(m_Transform);

			m_TrailSystem->Play();
			m_ExplosionSystem->Pause();
		}
		else if (!m_HasExploded)
		{
			m_HasExploded = true;

			m_TrailSystem->SetLooping(false);

			m_ExplosionSystem->ResetParticles();
			m_ExplosionSystem->Play();
		}

		m_TrailSystem->Update();
		m_ExplosionSystem->Update();
	}
}

void Firework::Draw()
{
	if (!m_Paused)
	{
		m_TrailSystem->Draw();

		if (m_HasExploded)
			m_ExplosionSystem->Draw();
	}
}

void Firework::Reset()
{
	m_Transform.translation.y = m_InitialPosition.y;
	UpdateModelValueOfTransform(m_Transform);

	m_Paused = false;
	m_HasExploded = false;
	m_TrailSystem->m_EmissionPosition = m_Transform.translation;
	m_TrailSystem->ResetParticles();
	m_TrailSystem->SetLooping(true);
	m_TrailSystem->Play();
	m_ExplosionSystem->ResetParticles();
	m_ExplosionSystem->Pause();
}

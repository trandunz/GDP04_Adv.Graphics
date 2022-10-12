#include "Cloth.h"
#include "DistanceJoint.h"
#include "ShaderLoader.h"

Cloth::Cloth(unsigned width, unsigned height, float spacing, glm::vec3 _startPos)
{
	m_Size.x = width;
	m_Size.y = height;
	m_Transform.translation = _startPos;
	UpdateModelValueOfTransform(m_Transform);
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

			if (y == 0) 
			{
				particle->TogglePinned();
			}

			m_Particles.emplace_back(particle);
		}
	}

	m_ShaderID = ShaderLoader::CreateShader(
		"PositionPassthrough.vert",
		"",
		"QuadPatch.tc",
		"DynamicQuad.te",
		"SingleTexture.frag");

	//m_ShaderID = ShaderLoader::CreateShader("SingleTexture.vert", "SingleTexture.frag");
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


	glUseProgram(m_ShaderID);

	UpdateModelValueOfTransform(m_Transform);

	ShaderLoader::SetUniform1f(std::move(m_ShaderID), "InnerFidelity", 10.0f);
	ShaderLoader::SetUniform1f(std::move(m_ShaderID), "OuterFidelity", 10.0f);
	ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "PVMMatrix", Statics::SceneCamera.GetPVMatrix() * m_Transform.transform);

	for (int i = 0; i < m_Size.x * m_Size.y; i++)
	{
		ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[" + std::to_string(i) + "]", m_Particles[i]->GetPosition());
	}


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	StaticMesh::Patch_Quad->Draw();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(0);
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

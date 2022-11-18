// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Cloth.cpp 
// Description : Cloth Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "Cloth.h"
#include "DistanceJoint.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"

Cloth::Cloth(unsigned width, unsigned height, float spacing, glm::vec3 _startPos)
{
	m_Size.x = (float)width;
	m_Size.y = (float)height;
	m_Transform.translation = _startPos;
	m_Spacing = spacing;
	m_HookCount = width;
	RingSpacing = spacing;
	UpdateModelValueOfTransform(m_Transform);

	CreateParticles(0,0, (unsigned)m_Size.x, (unsigned)m_Size.y);
	CreateConstraints(0,0, (unsigned)m_Size.x, (unsigned)m_Size.y);
	
	m_ShaderID = ShaderLoader::CreateShader("Normals3D.vert", "DynamicQuad.geo", "SingleTexture.frag");

	m_Texture = TextureLoader::LoadTexture("coollizard.png");
}

Cloth::~Cloth()
{
	for (auto& joint : m_DistanceJoints)
	{
		delete joint;
		joint = nullptr;
	}
	m_Particles.clear();
	m_DistanceJoints.clear();

	Plane = nullptr;
}

void Cloth::Update()
{
	HandleGroundCollision();
	HandleSelfCollision();
	HandleMouseInteraction();

	for (int y = 0; y < m_Size.y; y++)
	{
		for (int x = 0; x < m_Size.x; x++)
		{
			auto mass = m_Particles[Index(y, x)].GetMass();
			m_Particles[Index(y, x)].ApplyForce({ 0,-9.81f * mass, 0.0f});
			m_Particles[Index(y, x)].Update();
		}
	}

	for (auto& distanceJoint : m_DistanceJoints)
	{
		distanceJoint->Update();
	}
}

void Cloth::Draw()
{
	if (m_Particles.size() > 0)
	{
		if (DebugDraw)
		{
			for (auto& particle : m_Particles)
			{
				particle.Draw();
			}
		}

		glUseProgram(m_ShaderID);
		ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "PVMatrix", Statics::ActiveCamera.GetPVMatrix());

		glBindTexture(GL_TEXTURE_2D, m_Texture.ID);
		glActiveTexture(0);
		ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Texture0", 0);
		ShaderLoader::SetUniform1i(std::move(m_ShaderID), "TextureCount", 1);

		for (int y = 0; y < m_Size.y - 1; y++)
		{
			for (int x = 0; x < m_Size.x - 1; x++)
			{
				ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[0]", m_Particles[Index(y, x)].GetPosition());
				ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[1]", m_Particles[Index(y + 1, x)].GetPosition());
				ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[2]", m_Particles[Index(y + 1, x + 1)].GetPosition());
				ShaderLoader::SetUniform2f(std::move(m_ShaderID), "UniformTexCoords[0]", (x / m_Size.x), 1.0f - (y / m_Size.y));
				ShaderLoader::SetUniform2f(std::move(m_ShaderID), "UniformTexCoords[1]", (x / m_Size.x), 1.0f - ((y + 1) / m_Size.y));
				ShaderLoader::SetUniform2f(std::move(m_ShaderID), "UniformTexCoords[2]", ((x + 1) / m_Size.x), 1.0f - ((y + 1) / m_Size.y));
				StaticMesh::Triangle->Draw();

				ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[0]", m_Particles[Index(y + 1, x + 1)].GetPosition());
				ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[1]", m_Particles[Index(y, x + 1)].GetPosition());
				ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[2]", m_Particles[Index(y, x)].GetPosition());
				ShaderLoader::SetUniform2f(std::move(m_ShaderID), "UniformTexCoords[0]", ((x + 1) / m_Size.x), 1.0f - ((y + 1) / m_Size.y));
				ShaderLoader::SetUniform2f(std::move(m_ShaderID), "UniformTexCoords[1]", ((x + 1) / m_Size.x), 1.0f - (y / m_Size.y));
				ShaderLoader::SetUniform2f(std::move(m_ShaderID), "UniformTexCoords[2]", (x / m_Size.x), 1.0f - (y / m_Size.y));
				StaticMesh::Triangle->Draw();
			}
		}
		glUseProgram(0);
	}
}

int Cloth::GetWidth()
{
	return (int)m_Size.x;
}

int Cloth::GetHeight()
{
	return (int)m_Size.y;
}

int Cloth::GetHookCount()
{
	return m_HookCount;
}

void Cloth::SetGround(GameObject& _ground)
{
	Plane = &_ground;
}

void Cloth::SetHookCount(unsigned _amount)
{
	if (_amount != m_HookCount)
	{
		m_HookCount = _amount;
		UpdateHookCount();
	}
}

void Cloth::SetWidth(unsigned _amount)
{
	if (_amount != m_Size.x)
	{
		UpdateWidth(_amount);
	}
}

void Cloth::SetHeight(unsigned _amount)
{
	if (_amount != m_Size.y)
	{
		UpdateHeight(_amount);
	}
}

void Cloth::SetRingSpacing(float _spacing)
{
	if (_spacing != RingSpacing)
	{
		RingSpacing = _spacing;
		UpdateRingSpacing();
	}
}

void Cloth::SetWindDirection(glm::vec3 _direction)
{
	if (Wind != _direction)
	{
		Wind = _direction;

		for (auto& particle : m_Particles)
		{
			particle.Wind = Wind;
		}
	}
}

void Cloth::SetWindStrength(float _strength)
{
	if (glm::length(Wind) != _strength)
	{
		Wind = glm::normalize(Wind) * _strength;
		for (auto& particle : m_Particles)
		{
			particle.Wind = Wind;
		}
	}
}

void Cloth::SetDebugDraw(bool _drawPoints)
{
	if (DebugDraw != _drawPoints)
	{
		DebugDraw = _drawPoints;
	}
}

void Cloth::SetElasticity(float _amount)
{
	for (auto& distanceJoint : m_DistanceJoints)
	{
		if (distanceJoint->Stiffness != _amount)
		{
			distanceJoint->Stiffness = _amount;
		}
	}
}

float Cloth::GetElasticity()
{
	if (m_DistanceJoints.size() > 0)
	{
		return m_DistanceJoints[0]->Stiffness;
	}

	return 0.0f;
}

void Cloth::CheckCollision(Collider* _collider)
{
	if (!_collider)
		return;

	glm::vec3 collisionDirection{};
	bool collided{};
	for (auto& particle : m_Particles)
	{
		collided = Physics::SphereVSSphere(particle.Collider, *_collider, collisionDirection);
		if (collided)
		{
			particle.ApplyForce(collisionDirection * 100.0f);
		}
	}
}

void Cloth::HandleGroundCollision()
{
	if (Plane)
	{
		bool collided{};
		for (auto& particle : m_Particles)
		{
			collided = particle.GetPosition().y < Plane->GetTransform().translation.y;
			if (collided)
			{
				particle.ApplyForce(Up * 100.0f);
			}
		}
	}
}

void Cloth::HandleSelfCollision()
{
	glm::vec3 collisionDirection{};
	bool collided{};
	for (auto& particle : m_Particles)
	{
		for (auto& otherParticle : m_Particles)
		{
			if (particle.GetPosition() != otherParticle.GetPosition())
			{
				collided = Physics::SphereVSSphere(particle.Collider, otherParticle.Collider, collisionDirection);
				if (collided)
				{
					particle.ApplyForce(collisionDirection * 100.0f / 2.0f);
					otherParticle.ApplyForce(-collisionDirection * 100.0f / 2.0f);
				}
			}
		}
	}
}

void Cloth::CleanupParticlesAndJoints()
{
	m_Particles.clear();
	for (auto& joint : m_DistanceJoints)
	{
		delete joint;
	}
	m_DistanceJoints.clear();
}

void Cloth::UpdateHookCount()
{
	for (int x = 0; x < m_Size.x; x++)
	{
		if (m_HookCount == 0)
		{
			m_Particles[Index(0, x)].SetPinned(false);
		}
		else
		{
			if (x % (int)(m_Size.x / (float)m_HookCount) == 0)
			{
				m_Particles[Index(0, x)].SetPinned(true);
			}
			else
			{
				m_Particles[Index(0, x)].SetPinned(false);
			}
		}
	}
}

void Cloth::UpdateWidth(unsigned _newWidth)
{
	if (m_Size.x < _newWidth)
	{
		CreateParticles(0, 0, _newWidth, (unsigned)m_Size.y);
		CreateConstraints(0, 0, _newWidth, (unsigned)m_Size.y);
		m_Size.x = (float)_newWidth;
		m_HookCount = _newWidth;
	}
}

void Cloth::UpdateHeight(unsigned _newHeight)
{
	if (m_Size.y < _newHeight)
	{
		CreateParticles(0, 0, (unsigned)m_Size.x, _newHeight);
		CreateConstraints(0, 0, (unsigned)m_Size.x, _newHeight);
		m_Size.y = (float)_newHeight;
	}
}

void Cloth::UpdateRingSpacing()
{
	if (m_HookCount > 0)
	{
		for (int x = 0; x < m_Size.x; x++)
		{
			if (m_Particles[Index(0, x)].IsPinned())
			{
				auto newPos = m_Transform.translation;
				newPos.x += (x * RingSpacing);
				m_Particles[Index(0, x)].SetPosition(newPos);
				m_Particles[Index(0, x)].SetStartPos(newPos);
			}
		}
	}
}

void Cloth::CreateParticles(unsigned _startIndexX, unsigned _startIndexY, unsigned _width, unsigned _height)
{
	m_Particles.resize(_width * _height);

	for (int y = (int)_startIndexY; y < (int)_height; y++)
	{
		for (int x = (int)_startIndexX; x < (int)_width; x++)
		{
			m_Particles[Index(y, x)] = ClothParticle({ m_Transform.translation.x + (x * m_Spacing),m_Transform.translation.y - (y * m_Spacing),m_Transform.translation.z });
			if (y == 0)
			{
				m_Particles[Index(y, x)].TogglePinned();
			}
		}
	}
}

void Cloth::CreateConstraints(unsigned _startIndexX, unsigned _startIndexY, unsigned _width, unsigned _height)
{
	for (int y = (int)_startIndexY; y < (int)_height; y++)
	{
		for (int x = (int)_startIndexX; x< (int)_width - 1; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(&m_Particles[Index(y, x)], &m_Particles[Index(y, x + 1)], m_Spacing));
		}
	}
	for (int y = (int)_startIndexY; y < (int)_height - 1; y++)
	{
		for (int x = (int)_startIndexX; x < (int)_width; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(&m_Particles[Index(y, x)], &m_Particles[Index(y + 1, x)], m_Spacing));
		}
	}
	for (int y = (int)_startIndexY + 1; y < (int)_height; y++)
	{
		for (int x = (int)_startIndexX; x < (int)_width - 1; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(&m_Particles[Index(y, x)], &m_Particles[Index(y - 1, x + 1)], sqrtf(((m_Spacing * m_Spacing) * 2))));
		}
	}
	for (int y = (int)_startIndexY; y < (int)_height - 1; y++)
	{
		for (int x = (int)_startIndexX; x < (int)_width - 1; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(&m_Particles[Index(y, x)], &m_Particles[Index(y + 1, x + 1)], sqrtf(((m_Spacing * m_Spacing) * 2))));
		}
	}
	for (int y = (int)_startIndexY + 1; y < (int)_height; y++)
	{
		for (int x = (int)_startIndexX + 1; x < (int)_width; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(&m_Particles[Index(y, x)], &m_Particles[Index(y - 1, x - 1)], sqrtf(((m_Spacing * m_Spacing) * 2))));
		}
	}
	for (int y = (int)_startIndexY; y < (int)_height - 1; y++)
	{
		for (int x = (int)_startIndexX + 1; x < (int)_width; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(&m_Particles[Index(y, x)], &m_Particles[Index(y + 1, x - 1)], sqrtf(((m_Spacing * m_Spacing) * 2))));
		}
	}

	for (int y = (int)_startIndexY; y < (int)_height - 2; y++)
	{
		for (int x = (int)_startIndexX; x < (int)_width; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(&m_Particles[Index(y, x)], &m_Particles[Index(y + 2, x)], m_Spacing * 2));
		}
	}
	for (int y = (int)_startIndexY; y < (int)_height; y++)
	{
		for (int x = (int)_startIndexX; x < (int)_width - 2; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(&m_Particles[Index(y, x)], &m_Particles[Index(y, x + 2)], m_Spacing * 2));
		}
	}

}

void Cloth::HandleMouseInteraction()
{
	for (int y = 0; y < m_Size.y - 1; y++)
	{
		for (int x = 0; x < m_Size.x - 1; x++)
		{
			switch (InteractionType)
			{
			case INTERACTIONTYPE::PULL:
			{
				HandlePulling(x, y);
				break;
			}
			case INTERACTIONTYPE::PUSH:
			{
				HandlePushing(x, y);
				break;
			}
			default:
				break;
			}
		}
	}
}

void Cloth::HandlePushing(int _x, int _y)
{
	int state = glfwGetMouseButton(Statics::RenderWindow, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		Ray cursorRay = Statics::ActiveCamera.GetRayCursorRay();

		Transform triangleCenterTransform = m_Particles[Index(_y, _x)].GetTransform();
		glm::vec3 particleAPos = m_Particles[Index(_y, _x)].GetPosition();
		glm::vec3 particleBPos = m_Particles[Index(_y + 1, _x)].GetPosition();
		glm::vec3 particleCPos = m_Particles[Index(_y + 1, _x + 1)].GetPosition();
		triangleCenterTransform.translation = { ((particleAPos.x + particleBPos.x + particleCPos.x) / 3.0f),
							((particleAPos.y + particleBPos.y + particleCPos.y) / 3.0f),
							((particleAPos.z + particleBPos.z + particleCPos.z) / 3.0f) };
		UpdateModelValueOfTransform(triangleCenterTransform);

		if (Physics::IntersectMesh(StaticMesh::Triangle, triangleCenterTransform, cursorRay))
		{
			m_Particles[Index(_y, _x)].ApplyForce(cursorRay.direction * 1000.0f);
			m_Particles[Index(_y + 1, _x)].ApplyForce(cursorRay.direction * 1000.0f);
			m_Particles[Index(_y + 1, _x + 1)].ApplyForce(cursorRay.direction * 1000.0f);
		}

		particleAPos = m_Particles[Index(_y, _x)].GetPosition();
		particleBPos = m_Particles[Index(_y, _x + 1)].GetPosition();
		particleCPos = m_Particles[Index(_y + 1, _x + 1)].GetPosition();
		triangleCenterTransform.translation = { ((particleAPos.x + particleBPos.x + particleCPos.x) / 3.0f),
												((particleAPos.y + particleBPos.y + particleCPos.y) / 3.0f),
												((particleAPos.z + particleBPos.z + particleCPos.z) / 3.0f) };
		UpdateModelValueOfTransform(triangleCenterTransform);
		if (Physics::IntersectMesh(StaticMesh::Triangle, triangleCenterTransform, cursorRay))
		{
			m_Particles[Index(_y, _x)].ApplyForce(cursorRay.direction * 1000.0f);
			m_Particles[Index(_y, _x + 1)].ApplyForce(cursorRay.direction * 1000.0f);
			m_Particles[Index(_y + 1, _x + 1)].ApplyForce(cursorRay.direction * 1000.0f);
		}
	}
}

void Cloth::HandlePulling(int _x, int _y)
{
	int state = glfwGetMouseButton(Statics::RenderWindow, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		Ray cursorRay = Statics::ActiveCamera.GetRayCursorRay();

		Transform triangleCenterTransform = m_Particles[Index(_y, _x)].GetTransform();
		glm::vec3 particleAPos = m_Particles[Index(_y, _x)].GetPosition();
		glm::vec3 particleBPos = m_Particles[Index(_y + 1, _x)].GetPosition();
		glm::vec3 particleCPos = m_Particles[Index(_y + 1, _x + 1)].GetPosition();
		triangleCenterTransform.translation = { ((particleAPos.x + particleBPos.x + particleCPos.x) / 3.0f),
							((particleAPos.y + particleBPos.y + particleCPos.y) / 3.0f),
							((particleAPos.z + particleBPos.z + particleCPos.z) / 3.0f) };
		UpdateModelValueOfTransform(triangleCenterTransform);

		if (Physics::IntersectMesh(StaticMesh::Triangle, triangleCenterTransform, cursorRay))
		{
			m_Particles[Index(_y, _x)].ApplyForce(-cursorRay.direction * 1000.0f);
			m_Particles[Index(_y + 1, _x)].ApplyForce(-cursorRay.direction * 1000.0f);
			m_Particles[Index(_y + 1, _x + 1)].ApplyForce(-cursorRay.direction * 1000.0f);
		}

		particleAPos = m_Particles[Index(_y, _x)].GetPosition();
		particleBPos = m_Particles[Index(_y, _x + 1)].GetPosition();
		particleCPos = m_Particles[Index(_y + 1, _x + 1)].GetPosition();
		triangleCenterTransform.translation = { ((particleAPos.x + particleBPos.x + particleCPos.x) / 3.0f),
												((particleAPos.y + particleBPos.y + particleCPos.y) / 3.0f),
												((particleAPos.z + particleBPos.z + particleCPos.z) / 3.0f) };
		UpdateModelValueOfTransform(triangleCenterTransform);
		if (Physics::IntersectMesh(StaticMesh::Triangle, triangleCenterTransform, cursorRay))
		{
			m_Particles[Index(_y, _x)].ApplyForce(-cursorRay.direction * 1000.0f);
			m_Particles[Index(_y, _x + 1)].ApplyForce(-cursorRay.direction * 1000.0f);
			m_Particles[Index(_y + 1, _x + 1)].ApplyForce(-cursorRay.direction * 1000.0f);
		}
	}
}

ClothParticle::ClothParticle(glm::vec3 _startPos)
{
	m_Transform.translation = _startPos;
	m_PreviousPosition = _startPos;
	m_StartPosition = _startPos;
	SetMesh(StaticMesh::Sphere);
	SetShader("PositionOnly.vert", "UnlitColor.frag");
	SetTranslation(_startPos);
	SetScale({ 0.5f,0.5f,0.5f });

	Collider.Radius = 0.25f;
}

ClothParticle::~ClothParticle()
{
}

void ClothParticle::Update()
{
	if (!m_IsPinned)
	{
		ApplyForce(Wind);
		ApplyForce(-m_Velocity * m_Damping);

		auto position = m_Transform.translation;

		m_Transform.translation = ((1.0f + m_Damping) * m_Transform.translation) - (m_Damping * m_PreviousPosition) + (m_Acceleration * (FIXED_DT * FIXED_DT));

		m_PreviousPosition = position;

		m_Velocity = m_Transform.translation - m_PreviousPosition;
		m_Acceleration = {};
	}
	else
	{
		m_Transform.translation = m_StartPosition;
	}

	UpdateModelValueOfTransform(m_Transform);

	Collider.Center = m_Transform.translation;
}

void ClothParticle::Move(glm::vec3 _amount, bool _useDt)
{
	if (!m_IsPinned)
	{
		//float elasticity = std::lerp(60.0f, 1.0f, m_Elasticity);

		if (_useDt)
			m_Transform.translation += _amount * (FIXED_DT); //* elasticity;
		else
			m_Transform.translation += _amount;
	}
}

void ClothParticle::SetPosition(glm::vec3 _newPos)
{
	SetTranslation(_newPos);
}

void ClothParticle::SetStartPos(glm::vec3 _newPos)
{
	m_StartPosition = _newPos;
}

glm::vec3 ClothParticle::GetStartPos()
{
	return m_StartPosition;
}

void ClothParticle::ApplyForce(glm::vec3 _amount)
{
	if (!m_IsPinned)
	{
		m_Acceleration += (_amount / m_Mass);
	}
}

void ClothParticle::SetPinned(bool _pinned)
{
	m_IsPinned = _pinned;
}

void ClothParticle::TogglePinned()
{
	m_IsPinned = !m_IsPinned;
}

bool ClothParticle::IsPinned()
{
	return m_IsPinned;
}

float ClothParticle::GetMass()
{
	return m_Mass;
}

glm::vec3 ClothParticle::GetPosition() const
{
	return m_Transform.translation;
}

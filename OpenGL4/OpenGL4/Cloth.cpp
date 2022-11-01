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
	m_RingSpacing = spacing;
	UpdateModelValueOfTransform(m_Transform);

	CreateParticles(0,0, (unsigned)m_Size.x, (unsigned)m_Size.y);
	CreateConstraints(0,0, (unsigned)m_Size.x, (unsigned)m_Size.y);
	
	m_ShaderID = ShaderLoader::CreateShader("Normals3D.vert", "DynamicQuad.geo", "SingleTexture.frag");

	m_Texture = TextureLoader::LoadTexture("coollizard.png");
}

Cloth::~Cloth()
{
	CleanupParticlesAndJoints();

	m_Plane = nullptr;
}

void Cloth::Update()
{
	HandleGroundCollision();
	HandleSelfCollision();
	//HandleMouseInteraction();

	for (int y = 0; y < m_Size.y; y++)
	{
		for (int x = 0; x < m_Size.x; x++)
		{
			if (m_Particles[y][x])
			{
				auto mass = m_Particles[y][x]->GetMass();
				m_Particles[y][x]->ApplyForce({ 0,-9.81f * mass, 0.0f });
				m_Particles[y][x]->Update();
			}
		}
	}

	for (auto& distanceJoint : m_DistanceJoints)
	{
		if (distanceJoint)
		{
			distanceJoint->Update();
			if (distanceJoint->Destroy)
			{

				delete distanceJoint;
				distanceJoint = nullptr;
			}
		}
	}
}

void Cloth::Draw()
{
	if (sizeof(m_Particles) > 0)
	{
		if (m_DebugDraw)
		{
			for (int y = 0; y < m_Size.y; y++)
			{
				for (int x = 0; x < m_Size.x; x++)
				{
					if (m_Particles[y][x])
						m_Particles[y][x]->Draw();
				}
			}
		}

		//glUseProgram(m_ShaderID);
		//ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "PVMatrix", Statics::SceneCamera.GetPVMatrix());
		//
		//glBindTexture(GL_TEXTURE_2D, m_Texture.ID);
		//glActiveTexture(0);
		//ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Texture0", 0);
		//ShaderLoader::SetUniform1i(std::move(m_ShaderID), "TextureCount", 1);
		//
		//for (int y = 0; y < m_Size.y - 1; y++)
		//{
		//	for (int x = 0; x < m_Size.x - 1; x++)
		//	{
		//		if (m_Particles[y][x])
		//			ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[0]", m_Particles[y][x]->GetPosition());
		//		if (m_Particles[y + 1][x])
		//			ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[1]", m_Particles[y + 1][x]->GetPosition());
		//		if (m_Particles[y + 1][x + 1])
		//			ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[2]", m_Particles[y + 1][x + 1]->GetPosition());
		//		
		//		ShaderLoader::SetUniform2f(std::move(m_ShaderID), "UniformTexCoords[0]", (x / m_Size.x), 1.0f - (y / m_Size.y));
		//		ShaderLoader::SetUniform2f(std::move(m_ShaderID), "UniformTexCoords[1]", (x / m_Size.x), 1.0f - ((y + 1) / m_Size.y));
		//		ShaderLoader::SetUniform2f(std::move(m_ShaderID), "UniformTexCoords[2]", ((x + 1) / m_Size.x), 1.0f - ((y + 1) / m_Size.y));
		//		StaticMesh::Triangle->Draw();
		//
		//		if (m_Particles[y + 1][x + 1])
		//			ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[0]", m_Particles[y + 1][x + 1]->GetPosition());
		//		if (m_Particles[y][x + 1])
		//			ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[1]", m_Particles[y][x + 1]->GetPosition());
		//		if (m_Particles[y][x])
		//			ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[2]", m_Particles[y][x]->GetPosition());
		//		
		//		ShaderLoader::SetUniform2f(std::move(m_ShaderID), "UniformTexCoords[0]", ((x + 1) / m_Size.x), 1.0f - ((y + 1) / m_Size.y));
		//		ShaderLoader::SetUniform2f(std::move(m_ShaderID), "UniformTexCoords[1]", ((x + 1) / m_Size.x), 1.0f - (y / m_Size.y));
		//		ShaderLoader::SetUniform2f(std::move(m_ShaderID), "UniformTexCoords[2]", (x / m_Size.x), 1.0f - (y / m_Size.y));
		//		StaticMesh::Triangle->Draw();
		//	}
		//}
		//glUseProgram(0);

		for (auto& distanceJoint : m_DistanceJoints)
		{
			if (distanceJoint)
			{
				distanceJoint->Draw();
			}
		}
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
	m_Plane = &_ground;
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
	if (_spacing != m_RingSpacing)
	{
		m_RingSpacing = _spacing;
		UpdateRingSpacing();
	}
}

void Cloth::SetWindDirection(glm::vec3 _direction)
{
	if (m_Wind != _direction)
	{
		m_Wind = _direction;

		for (int y = 0; y < m_Size.y; y++)
		{
			for (int x = 0; x < m_Size.x; x++)
			{
				if (m_Particles[y][x])
				{
					m_Particles[y][x]->m_Wind = m_Wind;
				}
			}
		}
	}
}

void Cloth::SetWindStrength(float _strength)
{
	if (glm::length(m_Wind) != _strength)
	{
		m_Wind = glm::normalize(m_Wind) * _strength;
		for (int y = 0; y < m_Size.y; y++)
		{
			for (int x = 0; x < m_Size.x; x++)
			{
				if (m_Particles[y][x])
				{
					m_Particles[y][x]->m_Wind = m_Wind;
				}
			}
		}
	}
}

void Cloth::SetDebugDraw(bool _drawPoints)
{
	if (m_DebugDraw != _drawPoints)
	{
		m_DebugDraw = _drawPoints;
	}
}

void Cloth::SetElasticity(float _amount)
{
	for (auto& distanceJoint : m_DistanceJoints)
	{
		if (distanceJoint)
		{
			if (distanceJoint->m_Stiffness != _amount)
			{
				distanceJoint->m_Stiffness = _amount;
			}
		}
	}
}

float Cloth::GetElasticity()
{
	if (m_DistanceJoints.size() > 0)
	{
		return m_DistanceJoints[0]->m_Stiffness;
	}

	return 0.0f;
}

void Cloth::CheckCollision(Collider* _collider)
{
	if (!_collider)
		return;

	glm::vec3 collisionDirection{};
	bool collided{};
	for (int y = 0; y < m_Size.y; y++)
	{
		for (int x = 0; x < m_Size.x; x++)
		{
			if (m_Particles[y][x])
			{
				collided = Physics::SphereVSSphere(m_Particles[y][x]->Collider, *_collider, collisionDirection);
				if (collided)
				{
					m_Particles[y][x]->Move(collisionDirection * 10.0f);
				}
			}
		}
	}
}

void Cloth::HandleGroundCollision()
{
	if (m_Plane)
	{
		bool collided{};
		for (int y = 0; y < m_Size.y; y++)
		{
			for (int x = 0; x < m_Size.x; x++)
			{
				if (m_Particles[y][x])
				{
					collided = m_Particles[y][x]->GetPosition().y < m_Plane->GetTransform().translation.y;
					if (collided)
					{
						m_Particles[y][x]->Move(Up * 10.0f);
						m_Particles[y][x]->Move(Forward);
					}
				}
			}
		}
	}
}

void Cloth::HandleSelfCollision()
{
	glm::vec3 collisionDirection{};
	bool collided{};
	for (int y = 0; y < m_Size.y; y++)
	{
		for (int x = 0; x < m_Size.x; x++)
		{
			if (m_Particles[y][x])
			{
				for (int y2 = 0; y2 < m_Size.y; y2++)
				{
					for (int x2 = 0; x2 < m_Size.x; x2++)
					{
						if (m_Particles[y2][x2])
						{
							if (m_Particles[y][x]->GetPosition() != m_Particles[y2][x2]->GetPosition())
							{
								collided = Physics::SphereVSSphere(m_Particles[y][x]->Collider, m_Particles[y2][x2]->Collider, collisionDirection);
								if (collided)
								{
									m_Particles[y][x]->ApplyForce(collisionDirection * 100.0f / 2.0f);
									m_Particles[y2][x2]->ApplyForce(-collisionDirection * 100.0f / 2.0f);
								}
							}
						}
					}
				}
			}
		}
	}
}

void Cloth::CleanupParticlesAndJoints()
{
	for (int i = 0; i < m_Size.y; i++) 
	{
		for (int j = 0; j < m_Size.x; j++)
		{
			if (m_Particles[i][j])
			{
				delete m_Particles[i][j];
				m_Particles[i][j] = nullptr;
			}
		}
		delete[] m_Particles[i];
	}
	delete[] m_Particles;
	m_Particles = nullptr;

	for (auto& joint : m_DistanceJoints)
	{
		delete joint;
		joint = nullptr;
	}
	m_DistanceJoints.clear();
}

void Cloth::UpdateHookCount()
{
	for (int x = 0; x < m_Size.x; x++)
	{
		if (m_Particles[0][x])
		{
			if (m_HookCount == 0)
			{
				m_Particles[0][x]->SetPinned(false);
			}
			else
			{
				if ((int)(m_Size.x / (float)m_HookCount) != 0)
				{
					if (x % (int)(m_Size.x / (float)m_HookCount) == 0)
					{
						m_Particles[0][x]->SetPinned(true);
					}
					else
					{
						m_Particles[0][x]->SetPinned(false);
					}
				}
				else
				{
					m_Particles[0][x]->SetPinned(false);
				}
			}
		}
	}
}

void Cloth::UpdateWidth(unsigned _newWidth)
{
	if (m_Size.x != _newWidth)
	{
		CleanupParticlesAndJoints();
		m_Size.x = (float)_newWidth;
		m_HookCount = _newWidth;
		CreateParticles(0, 0, (unsigned)m_Size.x, (unsigned)m_Size.y);
		CreateConstraints(0, 0, (unsigned)m_Size.x, (unsigned)m_Size.y);
		UpdateHookCount();
	}
}

void Cloth::UpdateHeight(unsigned _newHeight)
{
	if (m_Size.y != _newHeight)
	{
		CleanupParticlesAndJoints();
		m_Size.y = (float)_newHeight;
		CreateParticles(0, 0, (unsigned)m_Size.x, (unsigned)m_Size.y);
		CreateConstraints(0, 0, (unsigned)m_Size.x, (unsigned)m_Size.y);
		UpdateHookCount();
	}
}

void Cloth::UpdateRingSpacing()
{
	if (m_HookCount > 0)
	{
		for (int x = 0; x < m_Size.x; x++)
		{
			if (m_Particles[0][x])
			{
				if (m_Particles[0][x]->IsPinned())
				{
					auto newPos = m_Transform.translation;
					newPos.x += (x * m_RingSpacing);
					m_Particles[0][x]->SetPosition(newPos);
					m_Particles[0][x]->SetStartPos(newPos);
				}
			}
		}
	}
}

void Cloth::CreateParticles(unsigned _startIndexX, unsigned _startIndexY, unsigned _width, unsigned _height)
{
	m_Particles = new ClothParticle** [_height];
	for (int i = 0; i < _height; i++)
		m_Particles[i] = new ClothParticle*[_width];

	for (int y = (int)_startIndexY; y < (int)_height; y++)
	{
		for (int x = (int)_startIndexX; x < (int)_width; x++)
		{
			m_Particles[y][x] = new ClothParticle({ m_Transform.translation.x + (x * m_Spacing),m_Transform.translation.y - (y * m_Spacing),m_Transform.translation.z });
			if (y == 0)
			{
				m_Particles[y][x]->TogglePinned();
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
			m_DistanceJoints.emplace_back(new DistanceJoint(m_Particles[y][x], m_Particles[y][x + 1], m_Spacing));
		}
	}
	for (int y = (int)_startIndexY; y < (int)_height - 1; y++)
	{
		for (int x = (int)_startIndexX; x < (int)_width; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(m_Particles[y][x], m_Particles[y + 1][x], m_Spacing));
		}
	}
	for (int y = (int)_startIndexY + 1; y < (int)_height; y++)
	{
		for (int x = (int)_startIndexX; x < (int)_width - 1; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(m_Particles[y][x], m_Particles[y - 1][x + 1], sqrtf(((m_Spacing * m_Spacing) * 2))));
		}
	}
	for (int y = (int)_startIndexY; y < (int)_height - 1; y++)
	{
		for (int x = (int)_startIndexX; x < (int)_width - 1; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(m_Particles[y][x], m_Particles[y + 1][x + 1], sqrtf(((m_Spacing * m_Spacing) * 2))));
		}
	}
	for (int y = (int)_startIndexY + 1; y < (int)_height; y++)
	{
		for (int x = (int)_startIndexX + 1; x < (int)_width; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(m_Particles[y][x], m_Particles[y - 1][x - 1], sqrtf(((m_Spacing * m_Spacing) * 2))));
		}
	}
	for (int y = (int)_startIndexY; y < (int)_height - 1; y++)
	{
		for (int x = (int)_startIndexX + 1; x < (int)_width; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(m_Particles[y][x], m_Particles[y + 1][x - 1], sqrtf(((m_Spacing * m_Spacing) * 2))));
		}
	}

	for (int y = (int)_startIndexY; y < (int)_height - 2; y++)
	{
		for (int x = (int)_startIndexX; x < (int)_width; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(m_Particles[y][x], m_Particles[y + 2][x], m_Spacing * 2));
		}
	}
	for (int y = (int)_startIndexY; y < (int)_height; y++)
	{
		for (int x = (int)_startIndexX; x < (int)_width - 2; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(m_Particles[y][x], m_Particles[y][x + 2], m_Spacing * 2));
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
	if (m_Particles[_y][_x])
	{
		int state = glfwGetMouseButton(Statics::RenderWindow, GLFW_MOUSE_BUTTON_LEFT);
		if (state == GLFW_PRESS)
		{
			Ray cursorRay = Statics::SceneCamera.GetRayCursorRay();

			Transform triangleCenterTransform = m_Particles[_y][_x]->GetTransform();
			glm::vec3 particleAPos = m_Particles[_y][_x]->GetPosition();
			glm::vec3 particleBPos = m_Particles[_y + 1][_x]->GetPosition();
			glm::vec3 particleCPos = m_Particles[_y + 1][_x + 1]->GetPosition();
			triangleCenterTransform.translation = { ((particleAPos.x + particleBPos.x + particleCPos.x) / 3.0f),
								((particleAPos.y + particleBPos.y + particleCPos.y) / 3.0f),
								((particleAPos.z + particleBPos.z + particleCPos.z) / 3.0f) };
			UpdateModelValueOfTransform(triangleCenterTransform);

			if (Physics::IntersectMesh(StaticMesh::Triangle, triangleCenterTransform, cursorRay))
			{
				m_Particles[_y][_x]->ApplyForce(cursorRay.direction * 1000.0f);
				m_Particles[_y + 1][_x]->ApplyForce(cursorRay.direction * 1000.0f);
				m_Particles[_y + 1][_x + 1]->ApplyForce(cursorRay.direction * 1000.0f);
			}

			particleAPos = m_Particles[_y][_x]->GetPosition();
			particleBPos = m_Particles[_y][_x + 1]->GetPosition();
			particleCPos = m_Particles[_y + 1][_x + 1]->GetPosition();
			triangleCenterTransform.translation = { ((particleAPos.x + particleBPos.x + particleCPos.x) / 3.0f),
													((particleAPos.y + particleBPos.y + particleCPos.y) / 3.0f),
													((particleAPos.z + particleBPos.z + particleCPos.z) / 3.0f) };
			UpdateModelValueOfTransform(triangleCenterTransform);
			if (Physics::IntersectMesh(StaticMesh::Triangle, triangleCenterTransform, cursorRay))
			{
				m_Particles[_y][_x]->ApplyForce(cursorRay.direction * 1000.0f);
				m_Particles[_y][_x + 1]->ApplyForce(cursorRay.direction * 1000.0f);
				m_Particles[_y + 1][_x + 1]->ApplyForce(cursorRay.direction * 1000.0f);
			}
		}
	}
}

void Cloth::HandlePulling(int _x, int _y)
{
	if (m_Particles[_y][_x])
	{
		int state = glfwGetMouseButton(Statics::RenderWindow, GLFW_MOUSE_BUTTON_LEFT);
		if (state == GLFW_PRESS)
		{
			Ray cursorRay = Statics::SceneCamera.GetRayCursorRay();

			Transform triangleCenterTransform = m_Particles[_y][_x]->GetTransform();
			glm::vec3 particleAPos = m_Particles[_y][_x]->GetPosition();
			glm::vec3 particleBPos = m_Particles[_y + 1][_x]->GetPosition();
			glm::vec3 particleCPos = m_Particles[_y + 1][_x + 1]->GetPosition();
			triangleCenterTransform.translation = { ((particleAPos.x + particleBPos.x + particleCPos.x) / 3.0f),
								((particleAPos.y + particleBPos.y + particleCPos.y) / 3.0f),
								((particleAPos.z + particleBPos.z + particleCPos.z) / 3.0f) };
			UpdateModelValueOfTransform(triangleCenterTransform);

			if (Physics::IntersectMesh(StaticMesh::Triangle, triangleCenterTransform, cursorRay))
			{
				m_Particles[_y][_x]->ApplyForce(-cursorRay.direction * 1000.0f);
				m_Particles[_y + 1][_x]->ApplyForce(-cursorRay.direction * 1000.0f);
				m_Particles[_y + 1][_x + 1]->ApplyForce(-cursorRay.direction * 1000.0f);
			}

			particleAPos = m_Particles[_y][_x]->GetPosition();
			particleBPos = m_Particles[_y][_x + 1]->GetPosition();
			particleCPos = m_Particles[_y + 1][_x + 1]->GetPosition();
			triangleCenterTransform.translation = { ((particleAPos.x + particleBPos.x + particleCPos.x) / 3.0f),
													((particleAPos.y + particleBPos.y + particleCPos.y) / 3.0f),
													((particleAPos.z + particleBPos.z + particleCPos.z) / 3.0f) };
			UpdateModelValueOfTransform(triangleCenterTransform);
			if (Physics::IntersectMesh(StaticMesh::Triangle, triangleCenterTransform, cursorRay))
			{
				m_Particles[_y][_x]->ApplyForce(-cursorRay.direction * 1000.0f);
				m_Particles[_y][_x + 1]->ApplyForce(-cursorRay.direction * 1000.0f);
				m_Particles[_y + 1][_x + 1]->ApplyForce(-cursorRay.direction * 1000.0f);
			}
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
		ApplyForce(m_Wind);
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

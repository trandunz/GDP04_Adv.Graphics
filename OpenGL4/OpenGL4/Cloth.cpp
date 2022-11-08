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
	
	m_ShaderID = ShaderLoader::CreateShader("Normals3D.vert", "DynamicQuad.geo", "SingleTexture_Coloured.frag");

	m_Texture = TextureLoader::LoadTexture("coollizard.png");
}

Cloth::~Cloth()
{
	CleanupParticlesAndJoints();

	Plane = nullptr;
}

void Cloth::Update()
{
	// Update elapsed time
	m_ElaspedTime += Statics::DeltaTime;

	ApplyRealisticWind();

	HandleGrabbedPoint();

	// Update all particles
	for (int y = 0; y < m_Size.y; y++)
	{
		for (int x = 0; x < m_Size.x; x++)
		{
			if (m_Particles[y][x])
			{
				if (m_Particles[y][x]->HasAnyJoints())
				{
					HandleFireSpread(x, y);

					m_Particles[y][x]->CheckForSingularJoints();

					float mass = m_Particles[y][x]->GetMass();
					m_Particles[y][x]->ApplyForce({ 0,-9.81f * mass, 0.0f });
				}
				m_Particles[y][x]->Update();
			}
		}
	}

	// Update all constraints
	for (auto& distanceJoint : m_DistanceJoints)
	{
		if (distanceJoint)
		{
			if (!distanceJoint->Destroy)
			{
				distanceJoint->Update();
			}
		}
	}

	HandleGroundCollision();
	HandleSelfCollision();
	HandleMouseInteraction();
}

void Cloth::Draw()
{
	if (m_Particles)
	{
		DebugDrawJoints();

		glUseProgram(m_ShaderID);
		ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "PVMatrix", Statics::SceneCamera.GetPVMatrix());
		
		glBindTexture(GL_TEXTURE_2D, m_Texture.ID);
		glActiveTexture(0);
		ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Texture0", 0);
		ShaderLoader::SetUniform1i(std::move(m_ShaderID), "TextureCount", 1);
		
		DrawTriangulatedMesh();
		glUseProgram(0);

		DrawFireParticles();
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
	if (Wind != _direction && !m_RealisticWind)
	{
		Wind = _direction;

		for (int y = 0; y < m_Size.y; y++)
		{
			for (int x = 0; x < m_Size.x; x++)
			{
				if (m_Particles[y][x])
				{
					m_Particles[y][x]->Wind = Wind;
				}
			}
		}
	}
}

void Cloth::SetWindStrength(float _strength)
{
	if (glm::length(Wind) >= 0.1f && glm::length(Wind) != _strength && !m_RealisticWind)
	{
		Wind = glm::normalize(Wind) * _strength;
		for (int y = 0; y < m_Size.y; y++)
		{
			for (int x = 0; x < m_Size.x; x++)
			{
				if (m_Particles[y][x])
				{
					m_Particles[y][x]->Wind = Wind;
				}
			}
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

void Cloth::SetRealisticWind(bool _niceWind)
{
	if (m_RealisticWind != _niceWind)
	{
		m_RealisticWind = _niceWind;
	}
}

void Cloth::ResetWind(bool resetNiceWind)
{
	if (m_RealisticWind != resetNiceWind)
	{
		m_RealisticWind = resetNiceWind;
		for (int y = 0; y < m_Size.y; y++)
		{
			for (int x = 0; x < m_Size.x; x++)
			{
				if (m_Particles[y][x])
				{
					m_Particles[y][x]->Wind = {};
				}
			}
		}
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
	if (Plane)
	{
		for (int y = 0; y < m_Size.y; y++)
		{
			for (int x = 0; x < m_Size.x; x++)
			{
				if (m_Particles[y][x])
				{
					if (m_Particles[y][x]->GetPosition().y <= Plane->GetTransform().translation.y + Statics::DeltaTime)
					{
						glm::vec3 newPos = m_Particles[y][x]->GetTransform().translation;
						newPos.y = Plane->GetTransform().translation.y + Statics::DeltaTime;
						m_Particles[y][x]->SetTranslation(newPos);
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
							// If particle is not the samee
							if (m_Particles[y][x]->GetPosition() != m_Particles[y2][x2]->GetPosition())
							{
								// check for collision
								collided = Physics::SphereVSSphere(m_Particles[y][x]->Collider, m_Particles[y2][x2]->Collider, collisionDirection);
								if (collided)
								{
									// Move them away from eachother
									m_Particles[y][x]->Move(collisionDirection * 10.0f / 2.0f);
									m_Particles[y2][x2]->Move(-collisionDirection * 10.0f / 2.0f);
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
		// Iterate along the top
		if (m_Particles[0][x])
		{
			// if hook count is 0 then unpin all 
			if (m_HookCount == 0)
			{
				m_Particles[0][x]->SetPinned(false);
			}
			else
			{
				// else iterate along the clotth and pin particles at every size / hookcount
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
		// Cleanup everything
		CleanupParticlesAndJoints();
		m_Size.x = (float)_newWidth;
		m_HookCount = _newWidth;

		// Create new meesh
		CreateParticles(0, 0, (unsigned)m_Size.x, (unsigned)m_Size.y);
		CreateConstraints(0, 0, (unsigned)m_Size.x, (unsigned)m_Size.y);
		// Align hook count with new size
		UpdateHookCount();
	}
}

void Cloth::UpdateHeight(unsigned _newHeight)
{
	if (m_Size.y != _newHeight)
	{
		// Cleanup everything
		CleanupParticlesAndJoints();
		m_Size.y = (float)_newHeight;
		// Create new mesh
		CreateParticles(0, 0, (unsigned)m_Size.x, (unsigned)m_Size.y);
		CreateConstraints(0, 0, (unsigned)m_Size.x, (unsigned)m_Size.y);
		// Align hook count with new size
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
					// set position to top left of cloth + particle index * ring spacing
					auto newPos = m_Transform.translation;
					newPos.x += (x * RingSpacing);
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
			
			// if at top of cloth then pin it
			if (y == 0)
			{
				m_Particles[y][x]->TogglePinned();
			}
		}
	}
}

void Cloth::CreateConstraints(unsigned _startIndexX, unsigned _startIndexY, unsigned _width, unsigned _height)
{
	// Horizontal
	for (int y = (int)_startIndexY; y < (int)_height; y++)
	{
		for (int x = (int)_startIndexX; x < (int)_width - 1; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(m_Particles[y][x], m_Particles[y][x + 1], m_Spacing));
			m_Particles[y][x]->AttachedJoints.push_back(m_DistanceJoints.back());
			m_Particles[y][x + 1]->AttachedJoints.push_back(m_DistanceJoints.back());
		}
	}
	// Vertical
	for (int y = (int)_startIndexY; y < (int)_height - 1; y++)
	{
		for (int x = (int)_startIndexX; x < (int)_width; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(m_Particles[y][x], m_Particles[y + 1][x], m_Spacing));
			m_Particles[y][x]->AttachedJoints.push_back(m_DistanceJoints.back());
			m_Particles[y + 1][x]->AttachedJoints.push_back(m_DistanceJoints.back());
		}
	}
	// -y, +x
	for (int y = (int)_startIndexY + 1; y < (int)_height; y++)
	{
		for (int x = (int)_startIndexX; x < (int)_width - 1; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(m_Particles[y][x], m_Particles[y - 1][x + 1], sqrtf(((m_Spacing * m_Spacing) * 2))));
			m_Particles[y][x]->DiagnalJoints.push_back(m_DistanceJoints.back());
			m_Particles[y - 1][x + 1]->DiagnalJoints.push_back(m_DistanceJoints.back());

		}
	}
	// +y, +x
	for (int y = (int)_startIndexY; y < (int)_height - 1; y++)
	{
		for (int x = (int)_startIndexX; x < (int)_width - 1; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(m_Particles[y][x], m_Particles[y + 1][x + 1], sqrtf(((m_Spacing * m_Spacing) * 2))));
			m_Particles[y][x]->BackwardDiagnals.push_back(m_DistanceJoints.back());
			m_Particles[y + 1][x + 1]->BackwardDiagnals.push_back(m_DistanceJoints.back());
		}
	}
	// -y, -x
	for (int y = (int)_startIndexY + 1; y < (int)_height; y++)
	{
		for (int x = (int)_startIndexX + 1; x < (int)_width; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(m_Particles[y][x], m_Particles[y - 1][x - 1], sqrtf(((m_Spacing * m_Spacing) * 2))));
			m_Particles[y][x]->BackwardDiagnals.push_back(m_DistanceJoints.back());
			m_Particles[y - 1][x - 1]->BackwardDiagnals.push_back(m_DistanceJoints.back());
		}
	}
	// +y,-x
	for (int y = (int)_startIndexY; y < (int)_height - 1; y++)
	{
		for (int x = (int)_startIndexX + 1; x < (int)_width; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(m_Particles[y][x], m_Particles[y + 1][x - 1], sqrtf(((m_Spacing * m_Spacing) * 2))));
			m_Particles[y][x]->DiagnalJoints.push_back(m_DistanceJoints.back());
			m_Particles[y + 1][x - 1]->DiagnalJoints.push_back(m_DistanceJoints.back());
		}
	}
	// vertical bend
	for (int y = (int)_startIndexY; y < (int)_height - 2; y++)
	{
		for (int x = (int)_startIndexX; x < (int)_width; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(m_Particles[y][x], m_Particles[y + 2][x], m_Spacing * 2));
			m_Particles[y][x]->BendJoints.push_back(m_DistanceJoints.back());
			m_Particles[y + 2][x]->BendJoints.push_back(m_DistanceJoints.back());
		}
	}
	// Horizontal bend
	for (int y = (int)_startIndexY; y < (int)_height; y++)
	{
		for (int x = (int)_startIndexX; x < (int)_width - 2; x++)
		{
			m_DistanceJoints.emplace_back(new DistanceJoint(m_Particles[y][x], m_Particles[y][x + 2], m_Spacing * 2));
			m_Particles[y][x]->BendJoints.push_back(m_DistanceJoints.back());
			m_Particles[y][x + 2]->BendJoints.push_back(m_DistanceJoints.back());
		}
	}
}

void Cloth::HandleMouseInteraction()
{
	for (int y = 0; y < m_Size.y; y++)
	{
		for (int x = 0; x < m_Size.x; x++)
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
			case INTERACTIONTYPE::GRAB:
			{
				HandleGrab(x, y);
				break;
			}
			case INTERACTIONTYPE::BURN:
			{
				ApplyBurn(x, y);
				break;
			}
			case INTERACTIONTYPE::TEAR:
			{
				HandleTear(x, y);
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

			if (Physics::IntersectMesh(StaticMesh::Triangle, triangleCenterTransform, cursorRay))
			{
				m_Particles[_y][_x]->ApplyForce(cursorRay.direction * 1000.0f);
				m_Particles[_y + 1][_x]->ApplyForce(cursorRay.direction * 1000.0f);
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

			if (Physics::IntersectMesh(StaticMesh::Triangle, triangleCenterTransform, cursorRay))
			{
				m_Particles[_y][_x]->ApplyForce(-cursorRay.direction * 1000.0f);
				m_Particles[_y + 1][_x]->ApplyForce(-cursorRay.direction * 1000.0f);
				m_Particles[_y][_x + 1]->ApplyForce(-cursorRay.direction * 1000.0f);
				m_Particles[_y + 1][_x + 1]->ApplyForce(-cursorRay.direction * 1000.0f);
			}
		}
	}
}

void Cloth::HandleGrab(int _x, int _y)
{
	if (m_Particles[_y][_x])
	{
		int state = glfwGetMouseButton(Statics::RenderWindow, GLFW_MOUSE_BUTTON_LEFT);
		if (state == GLFW_PRESS)
		{
			Ray cursorRay = Statics::SceneCamera.GetRayCursorRay();

			if (!m_PointIsGrabbed)
			{
				Transform triangleCenterTransform = m_Particles[_y][_x]->GetTransform();

				glm::vec3 hitPos{};
				if (Physics::IntersectMesh(StaticMesh::Triangle, triangleCenterTransform, cursorRay, hitPos))
				{
					m_PointIsGrabbed = true;
					m_GrabbedPoint = { _x ,  _y };
					m_StartGrabPosition = hitPos;
					m_GrabDistance = cursorRay.distance;
				}
			}
		}
		else
		{
			m_PointIsGrabbed = false;
		}
	}
}

void Cloth::HandleTear(int _x, int _y)
{
	if (m_Particles[_y][_x])
	{
		int state = glfwGetMouseButton(Statics::RenderWindow, GLFW_MOUSE_BUTTON_LEFT);
		if (state == GLFW_PRESS)
		{
			Ray cursorRay = Statics::SceneCamera.GetRayCursorRay();

			Transform triangleCenterTransform = m_Particles[_y][_x]->GetTransform();

			glm::vec3 hitPos{};
			if (Physics::IntersectMesh(StaticMesh::Triangle, triangleCenterTransform, cursorRay, hitPos))
			{
				m_Particles[_y][_x]->CleanupAllJoints();
			}
		}
	}
}

void Cloth::ApplyBurn(int _x, int _y)
{
	if (m_Particles[_y][_x])
	{
		int state = glfwGetMouseButton(Statics::RenderWindow, GLFW_MOUSE_BUTTON_LEFT);
		if (state == GLFW_PRESS)
		{
			Ray cursorRay = Statics::SceneCamera.GetRayCursorRay();

			Transform triangleCenterTransform = m_Particles[_y][_x]->GetTransform();

			glm::vec3 hitPos{};
			if (Physics::IntersectMesh(StaticMesh::Triangle, triangleCenterTransform, cursorRay, hitPos))
			{
				m_Particles[_y][_x]->IsBurning = true;
			}
		}
	}
}

void Cloth::ApplyRealisticWind()
{
	if (m_RealisticWind)
	{
		for (int y = 0; y < m_Size.y; y++)
		{
			for (int x = 0; x < m_Size.x; x++)
			{
				if (m_Particles[y][x])
				{
					m_Particles[y][x]->Wind = 
					{ 
						(cosf(m_ElaspedTime * 0.8f) * 2.0f), 
						(cosf(m_ElaspedTime * 0.8f) * 2.0f), 
						(sinf(m_ElaspedTime * 0.8f) * 2.0f) 
					};
				}
			}
		}
	}
}

void Cloth::HandleGrabbedPoint()
{
	if (m_PointIsGrabbed)
	{
		if (m_Size.x > m_GrabbedPoint.x && m_Size.y > m_GrabbedPoint.y)
		{
			if (m_Particles[m_GrabbedPoint.y][m_GrabbedPoint.x])
			{
				Ray cursorRay = Statics::SceneCamera.GetRayCursorRay();
				glm::vec3 newPos = cursorRay.origin + (glm::normalize(cursorRay.direction) * m_GrabDistance);
				m_Particles[m_GrabbedPoint.y][m_GrabbedPoint.x]->SetPosition(newPos);
			}
		}
	}
}

void Cloth::HandleFireSpread(int _x, int _y)
{
	// some random ish seed
	srand((int)time(NULL) * _x * _y + RandomFloat());

	// if the particle is burning and its health is less than its max health - a random range betwween 0 and 3
	if (m_Particles[_y][_x]->IsBurning && m_Particles[_y][_x]->Health < m_Particles[_y][_x]->MaxHealth - (rand() % 3))
	{
		// Choose a random number of surrounding triangles to also catch fire
		if (_y > 0 && _x > 0 && _x < m_Size.x - 1 && _y < m_Size.y - 1)
		{
			int randomChance = rand() % 6;
			if (randomChance == 5)
			{
				m_Particles[_y - 1][_x + 1]->IsBurning = true;
				m_Particles[_y - 1][_x - 1]->IsBurning = true;
				m_Particles[_y + 1][_x + 1]->IsBurning = true;
				m_Particles[_y][_x + 1]->IsBurning = true;
				m_Particles[_y + 1][_x]->IsBurning = true;
				m_Particles[_y + 1][_x - 1]->IsBurning = true;
			}
			else if (randomChance == 4)
			{
				m_Particles[_y - 1][_x + 1]->IsBurning = true;
				m_Particles[_y - 1][_x - 1]->IsBurning = true;
				m_Particles[_y + 1][_x + 1]->IsBurning = true;
				m_Particles[_y][_x + 1]->IsBurning = true;
				m_Particles[_y + 1][_x]->IsBurning = true;
			}
			else if (randomChance == 3)
			{
				m_Particles[_y - 1][_x + 1]->IsBurning = true;
				m_Particles[_y - 1][_x - 1]->IsBurning = true;
				m_Particles[_y + 1][_x + 1]->IsBurning = true;
				m_Particles[_y][_x + 1]->IsBurning = true;
			}
			else if (randomChance == 2)
			{
				m_Particles[_y - 1][_x + 1]->IsBurning = true;
				m_Particles[_y - 1][_x - 1]->IsBurning = true;
				m_Particles[_y + 1][_x + 1]->IsBurning = true;
			}
			else if (randomChance == 1)
			{
				m_Particles[_y - 1][_x + 1]->IsBurning = true;
				m_Particles[_y - 1][_x - 1]->IsBurning = true;
			}
			else if (randomChance == 0)
			{
				m_Particles[_y - 1][_x + 1]->IsBurning = true;
			}
		}
	}
}

void Cloth::DebugDrawJoints()
{
	if (DebugDraw)
	{
		for (int y = 0; y < m_Size.y; y++)
		{
			for (int x = 0; x < m_Size.x; x++)
			{
				if (m_Particles[y][x])
				{
					m_Particles[y][x]->Draw();
					for (auto& joint : m_Particles[y][x]->AttachedJoints)
					{
						// Draw white
						if (!joint->Destroy)
							joint->Draw();
					}
					for (auto& joint : m_Particles[y][x]->BendJoints)
					{
						// Draw red
						if (!joint->Destroy)
							joint->Draw({ 1,0,0 });
					}
					for (auto& joint : m_Particles[y][x]->DiagnalJoints)
					{
						// Draw green
						if (!joint->Destroy)
							joint->Draw({ 0,1,0 });
					}
					for (auto& joint : m_Particles[y][x]->BackwardDiagnals)
					{
						// draw blue
						if (!joint->Destroy)
							joint->Draw({ 0,0,1 });
					}
				}
			}
		}
	}
	
}

void Cloth::DrawTriangulatedMesh()
{
	for (int y = 0; y < m_Size.y - 1; y++)
	{
		for (int x = 0; x < m_Size.x - 1; x++)
		{
			bool render = true;
			// Check if any of the particles in the triangle have all there horizontal joints broken.
			// if so, destroy some joints from the surrounding particles too attempt a nice rip
			if (m_Particles[y][x]->AllJointsBroken())
			{
				render = false;
				m_Particles[y + 1][x]->CleanupBendJoints();
				m_Particles[y + 1][x + 1]->CleanupBendJoints();
				m_Particles[y + 1][x]->CleanupDiagnalJoints();
				m_Particles[y + 1][x + 1]->CleanupDiagnalJoints();
			}
			if (m_Particles[y + 1][x]->AllJointsBroken())
			{
				render = false;
				m_Particles[y + 1][x + 1]->CleanupBendJoints();
				m_Particles[y][x]->CleanupBendJoints();
				m_Particles[y + 1][x + 1]->CleanupDiagnalJoints();
				m_Particles[y][x]->CleanupDiagnalJoints();
			}
			if (m_Particles[y + 1][x + 1]->AllJointsBroken())
			{
				render = false;
				m_Particles[y + 1][x]->CleanupBendJoints();
				m_Particles[y][x]->CleanupBendJoints();
				m_Particles[y + 1][x]->CleanupDiagnalJoints();
				m_Particles[y][x]->CleanupDiagnalJoints();
			}

			// If the triangle still has joints then draw it
			if (render)
			{
				// Set positions
				ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[0]", m_Particles[y][x]->GetPosition());
				ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[1]", m_Particles[y + 1][x]->GetPosition());
				ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[2]", m_Particles[y + 1][x + 1]->GetPosition());
				// Set texture coords
				ShaderLoader::SetUniform2f(std::move(m_ShaderID), "UniformTexCoords[0]", (x / m_Size.x), 1.0f - (y / m_Size.y));
				ShaderLoader::SetUniform2f(std::move(m_ShaderID), "UniformTexCoords[1]", (x / m_Size.x), 1.0f - ((y + 1) / m_Size.y));
				ShaderLoader::SetUniform2f(std::move(m_ShaderID), "UniformTexCoords[2]", ((x + 1) / m_Size.x), 1.0f - ((y + 1) / m_Size.y));

				// Set color (for burning)
				ShaderLoader::SetUniform4fv(std::move(m_ShaderID), "Color", { m_Particles[y][x]->Health / m_Particles[y][x]->MaxHealth,m_Particles[y][x]->Health / m_Particles[y][x]->MaxHealth,m_Particles[y][x]->Health / m_Particles[y][x]->MaxHealth,1.0f });
				
				// Draw it
				StaticMesh::Triangle->Draw();
			}

			render = true;
			// Check if any of the particles in the triangle have all there horizontal joints broken.
			// if so, destroy some joints from the surrounding particles too attempt a nice rip
			if (m_Particles[y][x]->AllJointsBroken())
			{
				render = false;
				m_Particles[y + 1][x + 1]->CleanupBendJoints();
				m_Particles[y][x + 1]->CleanupBendJoints();
				m_Particles[y + 1][x + 1]->CleanupDiagnalJoints();
				m_Particles[y][x + 1]->CleanupDiagnalJoints();
			}
			if (m_Particles[y][x + 1]->AllJointsBroken())
			{
				render = false;
				m_Particles[y][x]->CleanupBendJoints();
				m_Particles[y + 1][x + 1]->CleanupBendJoints();
				m_Particles[y][x]->CleanupDiagnalJoints();
				m_Particles[y + 1][x + 1]->CleanupDiagnalJoints();
			}
			if (m_Particles[y + 1][x + 1]->AllJointsBroken())
			{
				render = false;
				m_Particles[y][x + 1]->CleanupBendJoints();
				m_Particles[y][x]->CleanupBendJoints();
				m_Particles[y][x + 1]->CleanupDiagnalJoints();
				m_Particles[y][x]->CleanupDiagnalJoints();
			}
			// If the triangle still has joints then draw it
			if (render)
			{
				// Set positions
				ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[0]", m_Particles[y + 1][x + 1]->GetPosition());
				ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[1]", m_Particles[y][x + 1]->GetPosition());
				ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "Points[2]", m_Particles[y][x]->GetPosition());
				// Set texture coords
				ShaderLoader::SetUniform2f(std::move(m_ShaderID), "UniformTexCoords[0]", ((x + 1) / m_Size.x), 1.0f - ((y + 1) / m_Size.y));
				ShaderLoader::SetUniform2f(std::move(m_ShaderID), "UniformTexCoords[1]", ((x + 1) / m_Size.x), 1.0f - (y / m_Size.y));
				ShaderLoader::SetUniform2f(std::move(m_ShaderID), "UniformTexCoords[2]", (x / m_Size.x), 1.0f - (y / m_Size.y));
				// Set color (for burning)
				ShaderLoader::SetUniform4fv(std::move(m_ShaderID), "Color", { m_Particles[y][x + 1]->Health / m_Particles[y][x + 1]->MaxHealth,m_Particles[y][x + 1]->Health / m_Particles[y][x + 1]->MaxHealth,m_Particles[y][x + 1]->Health / m_Particles[y][x + 1]->MaxHealth,1.0f });
				// Draw it
				StaticMesh::Triangle->Draw();
			}
		}
	}
}

void Cloth::DrawFireParticles()
{
	for (int y = 0; y < m_Size.y; y++)
	{
		for (int x = 0; x < m_Size.x; x++)
		{
			if (m_Particles[y][x])
			{
				if (m_Particles[y][x]->IsBurning)
				{
					if (m_Particles[y][x]->FireSystem)
					{
						m_Particles[y][x]->FireSystem->Draw();
					}
				}
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

	FireSystem = new ParticleSystem(m_Transform.translation, 0.1f);
	FireSystem->SetShader("PointToQuad.vert", "PointToQuad.geo", "SingleTexture_Coloured.frag");
	FireSystem->SetParticleTexture(TextureLoader::LoadTexture("Flame.png"));
	FireSystem->SetGravity(false);
	FireSystem->SetLifetime(0.5f);
	FireSystem->SetAlphaOverLifetime(1);
	FireSystem->Pause();
}

ClothParticle::~ClothParticle()
{
	if (FireSystem)
		delete FireSystem;
	FireSystem = nullptr;
}

void ClothParticle::Update()
{
	// If partle is dead then cleanup all its joints to prevent rendering and stop fire particle
	if (Health <= 0)
	{
		CleanupAllJoints();
		m_IsPinned = false;
		FireSystem->Stop();
	}
	// else if its alive and burning, decreease its head and play the fire particle system,
	if (IsBurning && Health > 0)
	{
		FireSystem->m_EmissionPosition = m_Transform.translation;
		Health -= Statics::DeltaTime;
		FireSystem->Play();
	}
	FireSystem->Update();

	// If its not pinned, performs verlet integration
	if (!m_IsPinned)
	{
		// Apply wind
		ApplyForce(Wind);

		// cache position
		glm::vec3 position = m_Transform.translation;
		// update real position using verlet
		m_Transform.translation = ((1.0f + m_Damping) * m_Transform.translation) - (m_Damping * m_PreviousPosition) + (m_Acceleration * (FIXED_DT * FIXED_DT));
		// assign previous position to cached position
		m_PreviousPosition = position;
		// calculate velocity
		m_Velocity = m_Transform.translation - m_PreviousPosition;
		// reset acccelleeration
		m_Acceleration = {};
	}
	// If its pinned, fix its position
	else
	{
		m_Transform.translation = m_StartPosition;
	}

	UpdateModelValueOfTransform(m_Transform);

	// Set the sphere collider to be at the particle
	Collider.Center = m_Transform.translation;
}

void ClothParticle::Move(glm::vec3 _amount, bool _useDt)
{
	if (!m_IsPinned)
	{
		if (_useDt)
			m_Transform.translation += _amount * (FIXED_DT);
		else
			m_Transform.translation += _amount;

		UpdateModelValueOfTransform(m_Transform);
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

bool ClothParticle::AllJointsBroken()
{
	for (auto& joint : AttachedJoints)
	{
		if (joint->Destroy == false)
		{
			return false;
		}
	}

	CleanupBendJoints();
	CleanupDiagnalJoints();
	return true;
}

void ClothParticle::CleanupHorizontalJoints()
{
	for (auto& joint : AttachedJoints)
	{
		joint->Destroy = true;
	}
}

void ClothParticle::CleanupAllJoints()
{
	CleanupHorizontalJoints();
	CleanupBendJoints();
	CleanupDiagnalJoints();
}

void ClothParticle::CleanupBendJoints()
{
	for (auto& joint : BendJoints)
	{
		joint->Destroy = true;
	}
}

void ClothParticle::CleanupDiagnalJoints()
{
	for (auto& joint : DiagnalJoints)
	{
		joint->Destroy = true;
	}
	for (auto& joint : BackwardDiagnals)
	{
		joint->Destroy = true;
	}
}

void ClothParticle::CheckForSingularJoints()
{
	CheckForSingularJoint(AttachedJoints);
	CheckForSingularJoint(BendJoints);
	CheckForSingularJoint(DiagnalJoints);
	CheckForSingularJoint(BackwardDiagnals);
}

void ClothParticle::CheckForSingularJoint(std::vector< DistanceJoint*>& _joints, int _count)
{
	int jointCount = 0;
	for (auto& joint : _joints)
	{
		if (joint->Destroy == false)
		{
			jointCount++;
		}
	}
	if (jointCount <= _count)
	{
		for (auto& joint : _joints)
		{
			joint->Destroy = true;
		}
	}
}

void ClothParticle::CheckForOnlyHorizontal()
{
	if (GetJointCount(BendJoints) <= 0
		&& GetJointCount(DiagnalJoints) <= 0
		&& GetJointCount(BackwardDiagnals) <= 0)
	{
		CheckForSingularJoint(AttachedJoints, 2);
	}
}

int ClothParticle::GetJointCount(std::vector<DistanceJoint*>& _joints)
{
	int jointCount = 0;
	for (auto& joint : _joints)
	{
		if (joint->Destroy == false)
		{
			jointCount++;
		}
	}
	return jointCount;
}

bool ClothParticle::HasAnyJoints()
{
	return GetJointCount(BendJoints) > 0
		|| GetJointCount(DiagnalJoints) > 0
		|| GetJointCount(BackwardDiagnals) > 0
		|| GetJointCount(AttachedJoints) > 0;
}

glm::vec3 ClothParticle::GetPosition() const
{
	return m_Transform.translation;
}

#include "Player.h"
#include "Physics.h"

Player::Player(glm::vec3 _startPos)
	: GameObject(_startPos)
{
}

Player::~Player()
{
	m_Ground = nullptr;
}

void Player::Update()
{
	Movement();
	m_ThirdPersonCamera.SetPosition(m_Transform.translation - (m_ThirdPersonCamera.GetFront() * 5.0f) + Up * 2.5f);
}

void Player::Movement()
{
	m_Velocity.x = 0;
	m_Velocity.z = 0;
	if (Statics::ActiveCursor || m_IsThirdPerson)
	{
		for (auto& key : Statics::Keymap)
		{
			if (key.second)
			{
				if (key.first == GLFW_KEY_W)
				{
					m_Velocity.z += -1;
				}
				if (key.first == GLFW_KEY_A)
				{
					m_Velocity.x += -1;
				}
				if (key.first == GLFW_KEY_S)
				{
					m_Velocity.z += 1;
				}
				if (key.first == GLFW_KEY_D)
				{
					m_Velocity.x += 1;
				}
			}
		}
	}

	if (m_IsThirdPerson && glm::length(glm::vec2(m_Velocity.x, m_Velocity.z)) > 0.1f)
	{
		glm::vec3 camForward = Statics::ActiveCamera->GetFront();
		camForward.y = 0;
		glm::vec3 camRight = Statics::ActiveCamera->GetRight();
		camRight.y = 0;

		glm::vec3 forwardRelativeInput = -m_Velocity.z * camForward;
		glm::vec3 rightRelativeInput = m_Velocity.x * camRight;
		glm::vec3 cameraRelativeMovement = forwardRelativeInput + rightRelativeInput;
		cameraRelativeMovement = glm::normalize(cameraRelativeMovement);
		m_Velocity.x = cameraRelativeMovement.x;
		m_Velocity.z = cameraRelativeMovement.z;
	}
	
	m_Velocity.x *= m_MoveSpeed;
	m_Velocity.z *= m_MoveSpeed;

	if (m_SkinnedMesh)
	{
		AlignWithInput();
		if (!m_IsGrounded)
		{
		}
		else if (glm::length(glm::vec2(m_Velocity.x, m_Velocity.z)) > 0.1f)
		{
			if (!m_IsMoving || m_SkinnedMesh->GetStartFrame() == 71)
			{
				m_SkinnedMesh->SetCurrentAnimation(31, 50);
			}
			m_IsMoving = true;
		}
		else
		{
			if (m_IsMoving || m_SkinnedMesh->GetStartFrame() == 71)
			{
				m_SkinnedMesh->SetCurrentAnimation(0, 30);
			}
			m_IsMoving = false;
		}
	}

	if (glm::length(m_Velocity) > 0.1f)
		Translate(m_Velocity * Statics::DeltaTime);

	HandleJump();
}

void Player::SetGround(GameObject* _ground)
{
	m_Ground = _ground;
}

void Player::ToggleThirdPersonCamera(bool _thirdPersonCamera)
{
	m_IsThirdPerson = _thirdPersonCamera;

	if (_thirdPersonCamera)
	{
		Statics::ActiveCamera = &m_ThirdPersonCamera;
	}
	else
	{
		Statics::ActiveCamera = &Statics::SceneCamera;
	}
}

void Player::AlignWithInput()
{
	if (m_IsThirdPerson)
	{
		SetRotation({ 0,1,0}, -Statics::ActiveCamera->GetYaw() - 90);
	}
	else
	{
		if (m_Velocity.z < 0 && m_Velocity.x > 0)
		{
			SetRotation({ 0,1,0 }, -45);
		}
		else if (m_Velocity.z > 0 && m_Velocity.x == 0)
		{
			SetRotation({ 0,1,0 }, 180);
		}
		else if (m_Velocity.z == 0 && m_Velocity.x > 0)
		{
			SetRotation({ 0,1,0 }, -90);
		}
		else if (m_Velocity.z == 0 && m_Velocity.x < 0)
		{
			SetRotation({ 0,1,0 }, 90);
		}
		else if (m_Velocity.z < 0 && m_Velocity.x < 0)
		{
			SetRotation({ 0,1,0 }, 45);
		}
		else if (m_Velocity.z > 0 && m_Velocity.x < 0)
		{
			SetRotation({ 0,1,0 }, 135);
		}
		else if (m_Velocity.z < 0 && m_Velocity.x == 0)
		{
			SetRotation({ 0,1,0 }, 0);
		}
		else if (m_Velocity.z > 0 && m_Velocity.x > 0)
		{
			SetRotation({ 0,1,0 }, 225);
		}
	}
}

void Player::CheckForGrounded()
{
	m_GroundedRay.origin = m_Transform.translation;
	m_GroundedRay.direction = {0,-1,0};
	if (m_Ground)
	{
		glm::vec3 _hitPoint{};
		if (Physics::IntersectMesh(m_Ground->GetMesh(), m_Ground->GetTransform(), m_GroundedRay, _hitPoint))
		{
			if (m_GroundedRay.distance <= 0.1f)
			{
				m_IsGrounded = true;
				return;
			}
		}
	}

	m_IsGrounded = false;
}

void Player::HandleJump()
{
	CheckForGrounded();

	if (m_IsGrounded)
	{
		m_Velocity.y = 0;
		SetTranslation({ m_Transform.translation.x, m_Ground->GetTransform().translation.y + 0.1f ,m_Transform.translation.z });
		for (auto& key : Statics::Keymap)
		{
			if (key.second)
			{
				if (key.first == GLFW_KEY_SPACE)
				{
					key.second = false;

					m_IsGrounded = false;
					m_Velocity.y = 10.0f;
					m_SkinnedMesh->SetCurrentAnimation(71, 80, false);
				}
			}
		}
	}
	else
	{
		m_Velocity.y -= Statics::DeltaTime * 9.81f;

	}
}

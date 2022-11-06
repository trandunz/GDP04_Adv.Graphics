// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Cloth.h 
// Description : Cloth Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "GameObject.h"
#include "Physics.h"
#include "ParticleSystem.h"

class DistanceJoint;
class ClothParticle : public GameObject
{
public:
	/// <summary>
	/// Cloth Particle Constructor
	/// </summary>
	/// <param name="_startPos"></param>
	ClothParticle(glm::vec3 _startPos = {0,0,-10});
	/// <summary>
	/// Cloth Particle Destructor
	/// </summary>
	virtual ~ClothParticle();

	/// <summary>
	/// Cloth Particle Update
	/// </summary>
	void Update() override;

	/// <summary>
	/// Move thr particle by the specified amount
	/// </summary>
	/// <param name="_amount"></param>
	/// <param name="_useDt"></param>
	void Move(glm::vec3 _amount, bool _useDt = true);
	void SetPosition(glm::vec3 _newPos);
	void SetStartPos(glm::vec3 _newPos);
	glm::vec3 GetStartPos();

	/// <summary>
	/// Apply a force to the particle
	/// </summary>
	/// <param name="_amount"></param>
	void ApplyForce(glm::vec3 _amount);

	/// <summary>
	/// Set whether or not the particle is pinned up
	/// </summary>
	/// <param name="_pinned"></param>
	void SetPinned(bool _pinned);
	/// <summary>
	/// Toggle whether or not the particle is pinned up
	/// </summary>
	void TogglePinned();

	bool IsPinned();
	float GetMass();

	bool AllJointsBroken();
	void CleanupAllJoints();
	void CleanupBendJoints();
	void CleanupDiagnalJoints();
	void CheckForSingularJoints();
	void CheckForSingularJoint(std::vector< DistanceJoint*>& _joints, int _count = 1);
	void CheckForOnlyHorizontal();
	int GetJointCount(std::vector< DistanceJoint*>& _joints);

	bool IsBurning{ false };
	float MaxHealth{ 10.0f };
	float Health{ 10.0f };
	glm::vec3 m_Wind{};
	float m_ConstraintLength{};
	Collider Collider{ {},0 };
	std::vector< DistanceJoint*> m_AttachedJoints{ };
	std::vector< DistanceJoint*> m_BendJoints{ };
	std::vector< DistanceJoint*> m_DiagnalJoints{ };
	std::vector< DistanceJoint*> m_BackwardDiagnals{ };
	ParticleSystem* m_FireSystem{ nullptr };
private:

	float m_Mass{ 1.0f };
	float m_Damping{ 0.99f };
	glm::vec3 m_Acceleration{};
	glm::vec3 m_Velocity{};
	bool m_IsPinned{ false };
	glm::vec3 m_PreviousPosition{};
	glm::vec3 m_StartPosition{};
	
public:
	glm::vec3 GetPosition() const;
};

class Cloth
{
public:
	enum class INTERACTIONTYPE
	{
		UNASSIGNED = 0,
		PULL,
		PUSH,
		GRAB,
		BURN,
		TEAR
	};

	/// <summary>
	/// Cloth Constructor
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="spacing"></param>
	/// <param name="_startPos"></param>
	Cloth(unsigned width, unsigned height, float spacing, glm::vec3 _startPos);
	/// <summary>
	/// Cloth Destructor
	/// </summary>
	~Cloth();

	/// <summary>
	/// Cloth update
	/// </summary>
	void Update();
	/// <summary>
	/// Draw the cloth
	/// </summary>
	void Draw();

	int GetWidth();
	int GetHeight();
	int GetHookCount();

	void SetGround(GameObject& _ground);

	void SetHookCount(unsigned _amount);
	void SetWidth(unsigned _amount);
	void SetHeight(unsigned _amount);
	void SetRingSpacing(float _spacing);
	void SetWindDirection(glm::vec3 _direction);
	void SetWindStrength(float _strength);
	void SetDebugDraw(bool _drawPoints);

	void SetElasticity(float _amount);
	float GetElasticity();

	void CheckCollision(Collider* _collider);

	INTERACTIONTYPE InteractionType{};
	float m_RingSpacing{ 1.0f };
	bool m_DebugDraw{ false };
	glm::vec3 m_Wind{};
	GameObject* m_Plane{ nullptr };
private:
	size_t Index(int x, int y) const { return (size_t)(x + (m_Size.x * y)); }

	void HandleGroundCollision();

	void HandleSelfCollision();

	/// <summary>
	/// Cleans up all particles and joints
	/// </summary>
	void CleanupParticlesAndJoints();

	/// <summary>
	/// Update the hook count
	/// </summary>
	void UpdateHookCount();
	/// <summary>
	/// Update the width of the cloth
	/// </summary>
	/// <param name="_newWidth"></param>
	void UpdateWidth(unsigned _newWidth);
	/// <summary>
	/// Update the height of the cloth
	/// </summary>
	/// <param name="_newWidth"></param>
	void UpdateHeight(unsigned _newWidth);
	/// <summary>
	/// Update the ring spacing of the cloth
	/// </summary>
	void UpdateRingSpacing();

	/// <summary>
	/// Creates particles
	/// </summary>
	/// <param name="_startIndexX"></param>
	/// <param name="_startIndexY"></param>
	/// <param name="_width"></param>
	/// <param name="_height"></param>
	void CreateParticles(unsigned _startIndexX, unsigned _startIndexY, unsigned _width, unsigned _height);
	/// <summary>
	/// Creates constraints
	/// </summary>
	/// <param name="_startIndexX"></param>
	/// <param name="_startIndexY"></param>
	/// <param name="_width"></param>
	/// <param name="_height"></param>
	void CreateConstraints(unsigned _startIndexX, unsigned _startIndexY, unsigned _width, unsigned _height);

	void HandleMouseInteraction();
	void HandlePushing(int _x, int _y);
	void HandlePulling(int _x, int _y);
	void HandleGrab(int _x, int _y);
	void ApplyBurn(int _x, int _y);

	bool m_PointIsGrabbed{};
	float m_GrabDistance{};
	glm::vec3 m_StartGrabPosition{};
	glm::ivec2 m_GrabbedPoint{};

	ClothParticle*** m_Particles{nullptr};
	std::vector<DistanceJoint*> m_DistanceJoints{};

	std::vector<unsigned int> m_Indices{};
	std::vector<Vertex> m_Vertices{};
	GLuint m_VertexArrayID{ 0 };
	GLuint m_VertexBufferID{ 0 };
	GLuint m_IndexBufferID{ 0 };
	float m_Spacing{};
	unsigned m_HookCount{};
	glm::vec2 m_Size{};
	Transform m_Transform{};
	GLuint m_ShaderID{};
	Texture m_Texture{};
};


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
	/// <summary>
	/// Set the position of the cloth particle
	/// </summary>
	/// <param name="_newPos"></param>
	void SetPosition(glm::vec3 _newPos);
	/// <summary>
	/// Set the starting position of the cloth particle
	/// </summary>
	/// <param name="_newPos"></param>
	void SetStartPos(glm::vec3 _newPos);
	/// <summary>
	/// Returns the position of the cloth particle
	/// </summary>
	/// <returns></returns>
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

	/// <summary>
	/// Returns if the particle is pinned up
	/// </summary>
	/// <returns></returns>
	bool IsPinned();
	/// <summary>
	/// Returns the mass of the particle
	/// </summary>
	/// <returns></returns>
	float GetMass();

	/// <summary>
	/// Checks if all the horizontal joints are broken
	/// </summary>
	/// <returns></returns>
	bool AllJointsBroken();
	/// <summary>
	/// Destroys all horizontal and vertical joints
	/// </summary>
	void CleanupHorizontalJoints();
	/// <summary>
	/// Destroys all joints
	/// </summary>
	void CleanupAllJoints();
	/// <summary>
	/// Destroys all bend joints
	/// </summary>
	void CleanupBendJoints();
	/// <summary>
	/// Destroys all diaganal joints
	/// </summary>
	void CleanupDiagnalJoints();
	/// <summary>
	/// Checks for singlular joints
	/// </summary>
	void CheckForSingularJoints();
	void CheckForSingularJoint(std::vector< DistanceJoint*>& _joints, int _count = 1);
	void CheckForOnlyHorizontal();
	/// <summary>
	/// Returns the number of alive joints in the specified vector
	/// </summary>
	/// <param name="_joints"></param>
	/// <returns></returns>
	int GetJointCount(std::vector< DistanceJoint*>& _joints);

	bool HasAnyJoints();

	bool IsBurning{ false };
	float MaxHealth{ 10.0f };
	float Health{ 10.0f };
	glm::vec3 Wind{};
	float ConstraintLength{};
	Collider Collider{ {},0 };
	std::vector< DistanceJoint*> AttachedJoints{ };
	std::vector< DistanceJoint*> BendJoints{ };
	std::vector< DistanceJoint*> DiagnalJoints{ };
	std::vector< DistanceJoint*> BackwardDiagnals{ };
	ParticleSystem* FireSystem{ nullptr };
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

	/// <summary>
	/// Returns the width of the cloth
	/// </summary>
	/// <returns></returns>
	int GetWidth();
	/// <summary>
	/// Returns the height of the cloth
	/// </summary>
	/// <returns></returns>
	int GetHeight();
	/// <summary>
	/// Returns the number of hooks present
	/// </summary>
	/// <returns></returns>
	int GetHookCount();

	/// <summary>
	/// Assign an object to be ground
	/// </summary>
	/// <param name="_ground"></param>
	void SetGround(GameObject& _ground);

	/// <summary>
	/// Set the number of hooks to be used on the cloth
	/// </summary>
	/// <param name="_amount"></param>
	void SetHookCount(unsigned _amount);
	/// <summary>
	/// Set the width of the cloth
	/// </summary>
	/// <param name="_amount"></param>
	void SetWidth(unsigned _amount);
	/// <summary>
	/// Set the height of the cloth
	/// </summary>
	/// <param name="_amount"></param>
	void SetHeight(unsigned _amount);
	/// <summary>
	/// Set the ring spacing between pinned particles in the cloth
	/// </summary>
	/// <param name="_spacing"></param>
	void SetRingSpacing(float _spacing);
	/// <summary>
	/// Set the winds direction
	/// </summary>
	/// <param name="_direction"></param>
	void SetWindDirection(glm::vec3 _direction);
	/// <summary>
	/// Set the winds strength
	/// </summary>
	/// <param name="_strength"></param>
	void SetWindStrength(float _strength);
	/// <summary>
	/// Doggle debug drawing
	/// </summary>
	/// <param name="_drawPoints"></param>
	void SetDebugDraw(bool _drawPoints);
	/// <summary>
	/// Toggle realistic wind effect
	/// </summary>
	/// <param name="_niceWind"></param>
	void SetRealisticWind(bool _niceWind);
	/// <summary>
	/// Resets the wind
	/// </summary>
	/// <param name="resetNiceWind"></param>
	void ResetWind(bool resetNiceWind);

	/// <summary>
	/// Set the elasticity of the cloth
	/// </summary>
	/// <param name="_amount"></param>
	void SetElasticity(float _amount);
	/// <summary>
	/// Returns the elasticity of the cloth
	/// </summary>
	/// <returns></returns>
	float GetElasticity();

	/// <summary>
	/// Checks for collision with the specified collider
	/// </summary>
	/// <param name="_collider"></param>
	void CheckCollision(Collider* _collider);

	INTERACTIONTYPE InteractionType{};
	float RingSpacing{ 1.0f };
	bool DebugDraw{ false };
	glm::vec3 Wind{};
	GameObject* Plane{ nullptr };
private:
	size_t Index(int x, int y) const { return (size_t)(x + (m_Size.x * y)); }

	/// <summary>
	/// Handles collision with the ground plane
	/// </summary>
	void HandleGroundCollision();

	/// <summary>
	/// Handles collision with itself
	/// </summary>
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

	/// <summary>
	/// Handles all mouse interactions with the cloth
	/// </summary>
	void HandleMouseInteraction();
	/// <summary>
	/// Handles the pushing mouse interaction
	/// </summary>
	/// <param name="_x"></param>
	/// <param name="_y"></param>
	void HandlePushing(int _x, int _y);
	/// <summary>
	/// Handles the pulling mouse interaction
	/// </summary>
	/// <param name="_x"></param>
	/// <param name="_y"></param>
	void HandlePulling(int _x, int _y);
	/// <summary>
	/// Handles the grabbing mouse interaction
	/// </summary>
	/// <param name="_x"></param>
	/// <param name="_y"></param>
	void HandleGrab(int _x, int _y);
	/// <summary>
	/// Handles the tearing mouse interaction
	/// </summary>
	/// <param name="_x"></param>
	/// <param name="_y"></param>
	void HandleTear(int _x, int _y);
	/// <summary>
	/// Applies burn to the specified particle
	/// </summary>
	/// <param name="_x"></param>
	/// <param name="_y"></param>
	void ApplyBurn(int _x, int _y);

	/// <summary>
	/// applies realistic wind using a fluctuating wind direction and strength
	/// </summary>
	void ApplyRealisticWind();

	/// <summary>
	/// If a point is grabbed then movee it too the mouse position
	/// </summary>
	void HandleGrabbedPoint();

	/// <summary>
	/// Handles the spreading of fire if the specified particle is currently burning
	/// </summary>
	/// <param name="_x"></param>
	/// <param name="_y"></param>
	void HandleFireSpread(int _x, int _y);

	/// <summary>
	/// Draws all joints if debug mode it enabled
	/// </summary>
	void DebugDrawJoints();

	/// <summary>
	/// Handles the constructing a drawing of the triangulated mesh for the cloth
	/// </summary>
	void DrawTriangulatedMesh();

	/// <summary>
	/// Handles drawing fire particles oon points that are burning
	/// </summary>
	void DrawFireParticles();

	bool m_PointIsGrabbed{};
	float m_GrabDistance{};
	glm::vec3 m_StartGrabPosition{};
	glm::ivec2 m_GrabbedPoint{};
	bool m_RealisticWind{ true };
	float m_ElaspedTime{};

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


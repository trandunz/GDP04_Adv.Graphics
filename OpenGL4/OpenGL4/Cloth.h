#pragma once
#include "GameObject.h"

class DistanceJoint;
class ClothParticle : public GameObject
{
public:
	ClothParticle(glm::vec3 _startPos = {0,0,-10});
	~ClothParticle();

	void Update() override;

	void Move(glm::vec3 _amount, bool _useDt = true);
	void SetPosition(glm::vec3 _newPos);

	void SetPinned(bool _pinned);
	void TogglePinned();
	bool IsPinned();

	float m_Elasticity{ 0.0f };
private:
	bool m_IsPinned{ false };
	glm::vec3 m_PreviousPosition{};
	glm::vec3 m_StartPosition{};
public:
	glm::vec3 GetPosition() const;
};

class Cloth
{
public:
	Cloth(unsigned width, unsigned height, float spacing, glm::vec3 _startPos);
	~Cloth();

	void Update();
	void Draw();

	int GetWidth();
	int GetHeight();
	int GetHookCount();
	void SetHookCount(unsigned _amount);
	void SetWidth(unsigned _amount);
	void SetHeight(unsigned _amount);
	void SetRingSpacing(float _spacing);

	void SetElasticity(float _amount);
	float GetElasticity();

	float m_RingSpacing{ 10.0f };
private:
	size_t Index(int x, int y) const { return x + m_Size.x * y; }

	void CleanupParticlesAndJoints();

	void UpdateHookCount();
	void UpdateWidth(unsigned _newWidth);
	void UpdateHeight(unsigned _newWidth);
	void UpdateRingSpacing();

	void CreateParticles(unsigned _startIndexX, unsigned _startIndexY, unsigned _width, unsigned _height);
	void CreateConstraints(unsigned _startIndexX, unsigned _startIndexY, unsigned _width, unsigned _height);

	std::vector<ClothParticle> m_Particles{};
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


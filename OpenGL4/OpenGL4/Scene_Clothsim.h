#pragma once
#include "Scene.h"
#include "Cloth.h"

class Scene_Clothsim : public Scene
{
public:
	Scene_Clothsim();
	~Scene_Clothsim();

	virtual void Start() override;
	virtual void Update() override;
	virtual void KeyEvents() override;
	virtual void CursorMoveEvent(double& xpos, double& ypos) override;
	virtual void CursorClickEvent(int button, int action, int mods) override;
	virtual void Draw() override;

private:
	void InitImGUI();
	void BindImGUI();
	void HandleDebugTools();
	void DrawImGUI();
	void CleanupImGUI();

	Cloth* m_Cloth{ nullptr };
	GameObject* m_FloorPlane{ nullptr };
	glm::vec2 m_CursorPos{};
	void* m_Io{ nullptr };
	
	bool m_DebugDraw = false;
	bool m_WireFrame = false;
	float m_ClothLength = 20.0f;
	float m_ClothWidth = 20.0f;
	int m_HookCount = m_ClothWidth;
	float m_HookDistance{1.0f};
	float m_Stiffness = 500.0f;
	int m_SelectedMouseMode{};
	int m_SelectedCollision{};
	const char* m_MouseModeItems[5]{ "Pull", "Push", "Tear", "Fire", "Pin" };
	const char* m_CollisionItems[4]{ "No Object", "Sphere", "Capsule", "Pyramid" };
	glm::vec3 m_WindDirection{};
	float m_WindStrength{ 0.0f };
};


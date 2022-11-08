// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Scene_Clothsim.h 
// Description : Scene_Clothsim Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

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
	/// <summary>
	/// Initialize ImGUI
	/// </summary>
	void InitImGUI();
	/// <summary>
	/// Bind ImGUI
	/// </summary>
	void BindImGUI();
	/// <summary>
	/// Create and render all the sliders, buttons, e.t.c.
	/// </summary>
	void HandleDebugTools();
	/// <summary>
	/// Draw all ImGUI Elements
	/// </summary>
	void DrawImGUI();
	/// <summary>
	/// Cleanup ImGUI
	/// </summary>
	void CleanupImGUI();

	void UpdateClothInteractionType();
	
	void HandleObjectInteraction();

	Cloth* m_Cloth{ nullptr };
	GameObject* m_FloorPlane{ nullptr };
	GameObject* m_CollisionSphere{ nullptr };
	glm::vec2 m_CursorPos{};
	void* m_Io{ nullptr };
	
	bool m_RealisticWind = true;
	bool m_DebugDraw = false;
	bool m_WireFrame = false;
	int m_ClothLength = 20;
	int m_ClothWidth = 20;
	int m_HookCount = m_ClothWidth;
	float m_HookDistance{1.0f};
	float m_Stiffness = 500.0f;
	int m_SelectedMouseMode{};
	int m_SelectedCollision{};
	const char* m_MouseModeItems[5]{ "Pull", "Push", "Tear", "Fire", "Grab" };
	const char* m_CollisionItems[4]{ "No Object", "Sphere", "Capsule", "Pyramid" };
	glm::vec3 m_WindDirection{};
	float m_WindStrength{ 0.0f };
};


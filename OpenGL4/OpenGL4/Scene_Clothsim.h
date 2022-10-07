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
};

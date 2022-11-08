#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Skybox.h"

class Scene_Assignment3 :
    public Scene
{
public:
	Scene_Assignment3();
	~Scene_Assignment3();

	virtual void Start() override;
	virtual void Update() override;
	virtual void KeyEvents() override;
	virtual void CursorMoveEvent(double& xpos, double& ypos) override;
	virtual void CursorClickEvent(int button, int action, int mods) override;
	virtual void Draw() override;

private:
	glm::vec2 m_CursorPos{};

	SkinnedMesh* m_AnimatedModel{ nullptr };

	GameObject* m_PlaneObject{ nullptr };
	GameObject* m_AssimpObject{ nullptr };
};


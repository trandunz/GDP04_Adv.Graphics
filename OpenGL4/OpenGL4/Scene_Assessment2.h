#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Skybox.h"

class Scene_Assessment2 : public Scene
{
public:
	Scene_Assessment2();
	~Scene_Assessment2();

	virtual void Start() override;
	virtual void Update() override;
	virtual void KeyEvents() override;
	virtual void CursorMoveEvent(double& xpos, double& ypos) override;
	virtual void CursorClickEvent(int button, int action, int mods) override;
	virtual void Draw() override;

private:
	glm::vec2 m_CursorPos{};
	
	GameObject* m_GeoStar{ nullptr };
	GameObject* m_NormalsSphere{ nullptr };
};


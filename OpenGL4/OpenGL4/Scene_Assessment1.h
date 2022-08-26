#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Skybox.h"
#include "Terrain.h"

class Scene_Assessment1 : public Scene
{
public:
	Scene_Assessment1();
	~Scene_Assessment1();

	virtual void Start() override;
	virtual void Update() override;
	virtual void KeyEvents() override;
	virtual void MouseEvents(double& xpos, double& ypos) override;
	virtual void Draw() override;

private:
	GameObject* m_ModelObject = nullptr;
	Mesh* m_ModelMesh = nullptr;
	Terrain* m_Terrain = nullptr;
};


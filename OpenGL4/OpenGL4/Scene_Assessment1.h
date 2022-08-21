#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "TextureLoader.h"
#include "Skybox.h"
#include "Terrain.h"
#include "Noise.h"

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
	bool BlackBars = true;
	GameObject* ModelObject = nullptr;
	Camera* SceneCamera = nullptr;
	Mesh* SphereMesh = nullptr;
	Mesh* CubeMesh = nullptr;
	Mesh* ModelMesh = nullptr;
	Terrain* TerrainMesh = nullptr;
	LightManager* lightManager = nullptr;
	Skybox* skyboxRef = nullptr;
};


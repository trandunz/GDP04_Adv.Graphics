// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Scene_Assessment1.h 
// Description : Scene_Assessment1 Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

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
	virtual void CursorMoveEvent(double& xpos, double& ypos) override;
	virtual void CursorClickEvent(int button, int action, int mods) override;
	virtual void Draw() override;

private:
	/// <summary>
	/// Handles all the interactions to do with mouse picking
	/// </summary>
	void HandleMousePickingInteractions();
	
	/// <summary>
	/// Billboards any object too the camera
	/// </summary>
	/// <param name="_object"></param>
	/// <param name="_relativePos"></param>
	/// <param name="_scale"></param>
	void BillboardObjectToCamera(GameObject& _object, glm::vec3 _relativePos, glm::vec3 _scale);

	GameObject* m_LeftQuad = nullptr;
	GameObject* m_RightQuad = nullptr;
	GameObject* m_FlatQuad = nullptr;
	GameObject* m_MousePickSphere = nullptr;
	GameObject* m_TerrainFollowingSphere = nullptr;
	GameObject* m_ModelObject = nullptr;
	GameObject* m_MossQuad = nullptr;

	Mesh* m_ModelMesh = nullptr;

	Terrain* m_LitTerrain = nullptr;
	Terrain* m_NoiseTerrain = nullptr;

	glm::vec2 m_CursorPos{};
};


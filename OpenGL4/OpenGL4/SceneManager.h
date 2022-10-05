// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : SceneManager.h 
// Description : SceneManager Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "Scene.h"

enum class SCENES
{
	UNASSIGNED = 0,

	ASSESSMENT1,
	ASSESSMENT2,
	CLOTHSIM
};

class SceneManager
{
public:
	static void Update();
	static void KeyEvents();
	static void CursorMoveEvent(double& xpos, double& ypos);
	static void CursorClickEvent(int button, int action, int mods);
	static void Draw();
	
	static void CleanupScene();
	static void LoadScene(SCENES _newLevel);
	static void ChangeToPendingScene();

	static float GetTimeSinceLoad();
private:
	SceneManager() {};
	~SceneManager() { CleanupScene(); };
	SceneManager(const SceneManager&) = delete;

	static Scene* CurrentScene;
	static SCENES SceneToLoad;
};


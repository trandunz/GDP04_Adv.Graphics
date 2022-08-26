#pragma once
#include "Scene.h"

enum class SCENES
{
	UNASSIGNED = 0,

	ASSESSMENT1
};

class SceneManager
{
public:
	static void Update();
	static void KeyEvents();
	static void MouseEvents(double& xpos, double& ypos);
	static void Draw();
	
	static void CleanupScene();
	static void LoadScene(SCENES _newLevel);
	static void ChangeToPendingScene();

private:
	SceneManager() {};
	~SceneManager() { CleanupScene(); };
	SceneManager(const SceneManager&) = delete;

	static Scene* CurrentScene;
	static SCENES SceneToLoad;
};


#include "SceneManager.h"
#include "Scene_Assessment1.h"

Scene* SceneManager::CurrentScene = nullptr;
SCENES SceneManager::SceneToLoad = SCENES::UNASSIGNED;

void SceneManager::Update()
{
	if (CurrentScene != nullptr)
		CurrentScene->Update();
}

void SceneManager::KeyEvents()
{
	if (CurrentScene != nullptr)
		CurrentScene->KeyEvents();
}

void SceneManager::MouseEvents(double& xpos, double& ypos)
{
	if (CurrentScene != nullptr)
		CurrentScene->MouseEvents(xpos, ypos);
}

void SceneManager::Draw()
{
	if (CurrentScene != nullptr)
		CurrentScene->Draw();
}

void SceneManager::CleanupScene()
{
	if (CurrentScene != nullptr)
	{
		delete CurrentScene;
		CurrentScene = nullptr;
	}
}

void SceneManager::LoadScene(SCENES _newLevel)
{
	SceneToLoad = _newLevel;
}

void SceneManager::ChangeToPendingScene()
{
	if (SceneToLoad != SCENES::UNASSIGNED)
	{
		CleanupScene();

		switch (SceneToLoad)
		{
		case SCENES::ASSESSMENT1:
		{
			CurrentScene = new Scene_Assessment1();
			break;
		}
		default:
			break;
		}

		SceneToLoad = SCENES::UNASSIGNED;
	}
}

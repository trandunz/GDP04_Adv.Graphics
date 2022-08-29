#pragma once
#include "Camera.h"

/// <summary>
/// Alias For Keymap (int = Key, bool = bPressed)
/// </summary>
using KEYMAP = std::map<int, bool>;

class Statics
{
public:
	static GLFWwindow* RenderWindow;
	static glm::ivec2 WindowSize;

	static float DeltaTime;
	static bool StencilTest;
	static bool Foggy;
	static bool BlackBars;
	static bool ActiveCursor;
	static Camera SceneCamera;
	static KEYMAP Keymap;

	static void CalculateDeltaTime();
	static void UpdateWindowSize();
	static void Cleanup();
private:
	static float LastTime;
};


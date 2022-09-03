// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Statics.h 
// Description : Statics Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

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
	static bool DSA;
	static Camera SceneCamera;
	static KEYMAP Keymap;

	/// <summary>
	/// Calculates delta time. Should be called at start of update.
	/// </summary>
	static void CalculateDeltaTime();
	/// <summary>
	/// Grabs the window size from GLFW. Should be called near start of update.
	/// </summary>
	static void UpdateWindowSize();
	/// <summary>
	/// Cleans up any neccessary pointers. E.G the GLFW Window
	/// </summary>
	static void Cleanup();
private:
	static float LastTime;
};


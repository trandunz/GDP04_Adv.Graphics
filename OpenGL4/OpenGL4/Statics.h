#pragma once
#include <GLFW/glfw3.h>
#include <map>
#include <glm/vec2.hpp>

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
	static KEYMAP Keymap;

	static void CalculateDeltaTime();
	static void UpdateWindowSize();
	static void Cleanup();
private:
	static float LastTime;
};


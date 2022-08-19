#pragma once
#include <GLFW/glfw3.h>
class Statics
{
public:
	static float DeltaTime;
	static bool StencilTest;

	static void CalculateDeltaTime();
private:
	static float LastTime;
};


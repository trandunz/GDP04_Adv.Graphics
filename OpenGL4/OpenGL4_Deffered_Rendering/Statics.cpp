// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Statics.cpp 
// Description : Statics Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "Statics.h"

glm::ivec2 Statics::WindowSize{};
GLFWwindow* Statics::RenderWindow = nullptr;
KEYMAP Statics::Keymap{};
Camera Statics::ActiveCamera{};
float Statics::DeltaTime{};
float Statics::LastTime{};
bool Statics::StencilTest{ false };
bool Statics::Foggy{ false };
bool Statics::BlackBars{ false };
bool Statics::ActiveCursor{ false };
bool Statics::DSA{ true };

void Statics::CalculateDeltaTime()
{
	float currentTime = (float)glfwGetTime();
	DeltaTime = currentTime - LastTime;
	LastTime = currentTime;
}

void Statics::UpdateWindowSize()
{
	if (RenderWindow != nullptr)
		glfwGetWindowSize(RenderWindow, &WindowSize.x, &WindowSize.y);
}

void Statics::Cleanup()
{
	if (RenderWindow != nullptr)
	{
		glfwDestroyWindow(Statics::RenderWindow);
		glfwTerminate();
		RenderWindow = nullptr;
	}
}

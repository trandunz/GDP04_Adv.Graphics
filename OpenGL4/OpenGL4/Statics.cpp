#include "Statics.h"

glm::ivec2 Statics::WindowSize{};
GLFWwindow* Statics::RenderWindow = nullptr;
KEYMAP Statics::Keymap{};
Camera Statics::SceneCamera{};
float Statics::DeltaTime{};
float Statics::LastTime{};
bool Statics::StencilTest{ true };
bool Statics::Foggy{ false };
bool Statics::BlackBars{ true };
bool Statics::ActiveCursor{ false };

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

#include "Statics.h"

float Statics::DeltaTime{};
float Statics::LastTime{};
bool Statics::StencilTest{ true };

void Statics::CalculateDeltaTime()
{
	float currentTime = (float)glfwGetTime();
	DeltaTime = currentTime - LastTime;
	LastTime = currentTime;
}

#pragma once
#include "Statics.h"
class Physics
{
public:
	static bool IntersectTriangle(Ray ray, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);
	static bool IntersectTriangle(Ray ray, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3& _intersectionPoint);

private:
	static float PointInOrOn(glm::vec3 P1, glm::vec3 P2, glm::vec3 A, glm::vec3 B);
	static bool PointInTriangle(glm::vec3 px, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);
	static glm::vec3 IntersectPlane(Ray ray, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);
};


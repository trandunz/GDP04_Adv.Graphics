// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Physics.h 
// Description : Physics Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz
// Shout out to : Gernot Steinegger (Line-Triangle Intersection)

#pragma once
#include "Statics.h"

#define FIXED_DT 1.0f/144.0f

struct SphereCollider
{
	inline SphereCollider(glm::vec3 _center, float _radius)
	{
		Center = _center;
		Radius = _radius;
	}

	inline SphereCollider()
	{
	}

	glm::vec3 Center{};
	float Radius{};
};

class Physics
{
public:
	/// <summary>
	/// Check for an intersection between the ray specified ray and the triangle defines by the three points
	/// </summary>
	/// <param name="_ray"></param>
	/// <param name="_p0"></param>
	/// <param name="_p1"></param>
	/// <param name="_p2"></param>
	/// <returns></returns>
	static bool IntersectTriangle(Ray _ray, glm::vec3 _p0, glm::vec3 _p1, glm::vec3 _p2);
	/// <summary>
	/// Check for an intersection between the ray specified ray and the triangle defines by the three points. Assigns the intersection point to _intersectionPoint
	/// </summary>
	/// <param name="_ray"></param>
	/// <param name="_p0"></param>
	/// <param name="_p1"></param>
	/// <param name="_p2"></param>
	/// <param name="_intersectionPoint"></param>
	/// <returns></returns>
	static bool IntersectTriangle(Ray _ray, glm::vec3 _p0, glm::vec3 _p1, glm::vec3 _p2, glm::vec3& _intersectionPoint);

	/// <summary>
	/// Checks if the point _px is inside the triangle defined by _p1, _p2, _p3
	/// </summary>
	/// <param name="_px"></param>
	/// <param name="_p0"></param>
	/// <param name="_p1"></param>
	/// <param name="_p2"></param>
	/// <returns></returns>
	static bool PointInTriangle(glm::vec3 _px, glm::vec3 _p0, glm::vec3 _p1, glm::vec3 _p2);

	static bool SphereVSSphere(SphereCollider& _sphere, SphereCollider& _otherSphere);
	static bool SphereVSSphere(SphereCollider& _sphere, SphereCollider& _otherSphere, glm::vec3& _resolutionDirection);
private:
	/// <summary>
	/// Returns an interpolated value between 0 and the dot product of the cross product of (_b - _a, _p1 - _a) and (_b - _a, _p2 - _a)
	/// </summary>
	/// <param name="_p1"></param>
	/// <param name="_p2"></param>
	/// <param name="_a"></param>
	/// <param name="_b"></param>
	/// <returns></returns>
	static float PointInOrOn(glm::vec3 _p1, glm::vec3 _p2, glm::vec3 _a, glm::vec3 _b);

	/// <summary>
	/// Checks if the ray intersects the plane with points on it _p0, _p1, _p2
	/// </summary>
	/// <param name="_ray"></param>
	/// <param name="_p0"></param>
	/// <param name="_p1"></param>
	/// <param name="_p2"></param>
	/// <returns></returns>
	static glm::vec3 IntersectPlane(Ray _ray, glm::vec3 _p0, glm::vec3 _p1, glm::vec3 _p2);
};


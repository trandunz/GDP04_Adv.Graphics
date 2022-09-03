// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Physics.cpp 
// Description : Physics Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz
// Shout out to : Gernot Steinegger (Line-Triangle Intersection)

#include "Physics.h"

float Physics::PointInOrOn(glm::vec3 _p1, glm::vec3 _p2, glm::vec3 _a, glm::vec3 _b)
{
    glm::vec3 cross1 = glm::cross(_b - _a, _p1 - _a);
    glm::vec3 cross2 = glm::cross(_b - _a, _p2 - _a);
    return glm::step(0.0f, glm::dot(cross1, cross2));
}

bool Physics::PointInTriangle(glm::vec3 _px, glm::vec3 _p0, glm::vec3 _p1, glm::vec3 _p2)
{
    return PointInOrOn(_px, _p0, _p1, _p2) * PointInOrOn(_px, _p1, _p2, _p0) * PointInOrOn(_px, _p2, _p0, _p1);
}

glm::vec3 Physics::IntersectPlane(Ray _ray, glm::vec3 _p0, glm::vec3 _p1, glm::vec3 _p2)
{
    glm::vec3 normal = glm::cross(_p1 - _p0, _p2 - _p0);
    glm::vec3 intersection = _ray.origin + _ray.direction * glm::dot(_p0 - _ray.origin, normal) / glm::dot(_ray.direction, normal);

    return intersection;
}

bool Physics::IntersectTriangle(Ray _ray, glm::vec3 _p0, glm::vec3 _p1, glm::vec3 _p2)
{
    glm::vec3 intersectionPoint = IntersectPlane(_ray, _p0, _p1, _p2);
    return PointInTriangle(intersectionPoint, _p0, _p1, _p2);
}

bool Physics::IntersectTriangle(Ray _ray, glm::vec3 _p0, glm::vec3 _p1, glm::vec3 _p2, glm::vec3& _intersectionPoint)
{
    glm::vec3 intersectionPoint = IntersectPlane(_ray, _p0, _p1, _p2);
    bool intersection = PointInTriangle(intersectionPoint, _p0, _p1, _p2);

    if (intersection)
        _intersectionPoint = intersectionPoint;

    return intersection;
}

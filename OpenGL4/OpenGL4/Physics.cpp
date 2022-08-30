#include "Physics.h"

float Physics::PointInOrOn(glm::vec3 P1, glm::vec3 P2, glm::vec3 A, glm::vec3 B)
{
    glm::vec3 CP1 = glm::cross(B - A, P1 - A);
    glm::vec3 CP2 = glm::cross(B - A, P2 - A);
    return glm::step(0.0f, glm::dot(CP1, CP2));
}

bool Physics::PointInTriangle(glm::vec3 px, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2)
{
    return
        PointInOrOn(px, p0, p1, p2) *
        PointInOrOn(px, p1, p2, p0) *
        PointInOrOn(px, p2, p0, p1);
}

glm::vec3 Physics::IntersectPlane(Ray ray, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2)
{
    glm::vec3 D = ray.direction;
    glm::vec3 N = glm::cross(p1 - p0, p2 - p0);
    glm::vec3 X = ray.origin + D * glm::dot(p0 - ray.origin, N) / glm::dot(D, N);

    return X;
}

bool Physics::IntersectTriangle(Ray ray, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2)
{
    glm::vec3 X = IntersectPlane(ray, p0, p1, p2);
    return PointInTriangle(X, p0, p1, p2);
}

bool Physics::IntersectTriangle(Ray ray, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3& _intersectionPoint)
{
    glm::vec3 X = IntersectPlane(ray, p0, p1, p2);
    bool intersection = PointInTriangle(X, p0, p1, p2);

    if (intersection)
        _intersectionPoint = X;

    return intersection;
}

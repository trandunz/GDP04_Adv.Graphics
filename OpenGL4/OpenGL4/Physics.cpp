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
#include "Mesh.h"

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

bool Physics::SphereVSSphere(Collider& _sphere, Collider& _otherSphere)
{
    return glm::distance(_sphere.Center, _otherSphere.Center) < (_sphere.Radius + _otherSphere.Radius);
}

bool Physics::SphereVSSphere(Collider& _sphere, Collider& _otherSphere, glm::vec3& _resolutionDirection)
{
    _resolutionDirection = _sphere.Center - _otherSphere.Center;

    return glm::distance(_sphere.Center, _otherSphere.Center) < (_sphere.Radius + _otherSphere.Radius);
}

bool Physics::IntersectMesh(Mesh* _mesh, Transform _meshTransform, Ray _ray)
{
    bool intersection = false;
    // If a mesh is assigned
    if (_mesh)
    {
        // Get refrence too indices and vertices
        std::vector<Vertex>* vertices = &_mesh->GetVertices();
        std::vector<unsigned>* indices = &_mesh->GetIndices();

        int index{ 0 };
        unsigned element{ 0 };
        // Loop over all indices and get points for each triangle in vertices
        for (unsigned i = 0; i < indices->size() / 3; i++)
        {
            element = (*indices)[index];
            glm::vec4 point1{ (*vertices)[element].position, 1.0f };
            point1 = _meshTransform.transform * point1;

            element = (*indices)[index + 1];
            glm::vec4 point2{ (*vertices)[element].position, 1.0f };
            point2 = _meshTransform.transform * point2;

            element = (*indices)[index + 2];
            glm::vec4 point3{ (*vertices)[element].position, 1.0f };
            point3 = _meshTransform.transform * point3;

            // test for intersection with triangle
            intersection = Physics::IntersectTriangle(_ray, point1, point2, point3);
            if (intersection)
                return true;

            index += 3;
        }
        vertices = nullptr;
        indices = nullptr;
    }

    return intersection;
}

bool Physics::IntersectMesh(Mesh* _mesh, Transform _meshTransform, Ray& _ray, glm::vec3& _hitPoint)
{
    bool intersection = false;
    // If a mesh is assigned
    if (_mesh)
    {
        // Get refrence too indices and vertices
        std::vector<Vertex>* vertices = &_mesh->GetVertices();
        std::vector<unsigned>* indices = &_mesh->GetIndices();

        int index{ 0 };
        unsigned element{ 0 };
        // Loop over all indices and get points for each triangle in vertices
        for (unsigned i = 0; i < indices->size() / 3; i++)
        {
            element = (*indices)[index];
            glm::vec4 point1{ (*vertices)[element].position, 1.0f };
            point1 = _meshTransform.transform * point1;

            element = (*indices)[index + 1];
            glm::vec4 point2{ (*vertices)[element].position, 1.0f };
            point2 = _meshTransform.transform * point2;

            element = (*indices)[index + 2];
            glm::vec4 point3{ (*vertices)[element].position, 1.0f };
            point3 = _meshTransform.transform * point3;

            // test for intersection with triangle
            intersection = Physics::IntersectTriangle(_ray, point1, point2, point3, _hitPoint);
            if (intersection)
            {
                _ray.distance = glm::length(_hitPoint - _ray.origin);
                return true;
            }
                

            index += 3;
        }
        vertices = nullptr;
        indices = nullptr;
    }

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

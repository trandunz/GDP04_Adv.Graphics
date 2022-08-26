#include "StaticMesh.h"

Mesh* StaticMesh::Cube{ nullptr };
Mesh* StaticMesh::InvertedCube{ nullptr };
Mesh* StaticMesh::Sphere{ nullptr };
Mesh* StaticMesh::Hemisphere{ nullptr };
Mesh* StaticMesh::Pyramid{ nullptr };

void StaticMesh::Init()
{
	Cleanup();
	InvertedCube = new Mesh(SHAPE::CUBE, GL_CW);
	Cube = new Mesh(SHAPE::CUBE);
	Sphere = new Mesh(SHAPE::SPHERE);
	Hemisphere = new Mesh(SHAPE::HEMISPHERE);
	Pyramid = new Mesh(SHAPE::PYRAMID);
}

void StaticMesh::Cleanup()
{
	if (Cube != nullptr)
	{
		delete Cube;
		Cube = nullptr;
	}
	if (InvertedCube != nullptr)
	{
		delete InvertedCube;
		InvertedCube = nullptr;
	}
	if (Sphere != nullptr)
	{
		delete Sphere;
		Sphere = nullptr;
	}
	if (Hemisphere != nullptr)
	{
		delete Hemisphere;
		Hemisphere = nullptr;
	}
	if (Pyramid != nullptr)
	{
		delete Pyramid;
		Pyramid = nullptr;
	}
}

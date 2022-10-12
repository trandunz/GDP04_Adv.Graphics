// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : StaticMesh.cpp 
// Description : StaticMesh Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "StaticMesh.h"

Mesh* StaticMesh::Point{ nullptr };
Mesh* StaticMesh::Quad{ nullptr };
Mesh* StaticMesh::Cube{ nullptr };
Mesh* StaticMesh::InvertedCube{ nullptr };
Mesh* StaticMesh::Sphere{ nullptr };
Mesh* StaticMesh::Hemisphere{ nullptr };
Mesh* StaticMesh::Pyramid{ nullptr };
Mesh* StaticMesh::Patch_Triangle{ nullptr };
Mesh* StaticMesh::Patch_Triangle_Quad{ nullptr };
Mesh* StaticMesh::Patch_Quad{ nullptr };
Mesh* StaticMesh::Plane{ nullptr };

void StaticMesh::Init()
{
	Cleanup();
	Patch_Triangle_Quad = new Mesh(SHAPE::PATCH_TRIANGLE_QUAD);
	Patch_Triangle = new Mesh(SHAPE::PATCH_TRIANGLE);
	Point = new Mesh(SHAPE::POINT);
	Quad = new Mesh(4);
	InvertedCube = new Mesh(SHAPE::CUBE, GL_CW);
	Cube = new Mesh(SHAPE::CUBE);
	Sphere = new Mesh(SHAPE::SPHERE, GL_CCW);
	Hemisphere = new Mesh(SHAPE::HEMISPHERE);
	Pyramid = new Mesh(SHAPE::PYRAMID);
	Plane = new Mesh(SHAPE::PLANE);
	Patch_Quad = new Mesh(SHAPE::PATCH_QUAD);
}

void StaticMesh::Cleanup()
{
	if (Quad != nullptr)
	{
		delete Quad;
		Quad = nullptr;
	}
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
	if (Point != nullptr)
	{
		delete Point;
		Point = nullptr;
	}
	if (Patch_Triangle != nullptr)
	{
		delete Patch_Triangle;
		Patch_Triangle = nullptr;
	}
	if (Patch_Triangle_Quad != nullptr)
	{
		delete Patch_Triangle_Quad;
		Patch_Triangle_Quad = nullptr;
	}
	if (Plane != nullptr)
	{
		delete Plane;
		Plane = nullptr;
	}
	if (Patch_Quad != nullptr)
	{
		delete Patch_Quad;
		Patch_Quad = nullptr;
	}
}

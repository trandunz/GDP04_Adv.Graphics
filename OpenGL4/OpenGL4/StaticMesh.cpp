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

void StaticMesh::Init()
{
	Cleanup();
	Point = new Mesh(SHAPE::POINT);
	Quad = new Mesh(4);
	InvertedCube = new Mesh(SHAPE::CUBE, GL_CW);
	Cube = new Mesh(SHAPE::CUBE);
	Sphere = new Mesh(SHAPE::SPHERE, GL_CCW);
	Hemisphere = new Mesh(SHAPE::HEMISPHERE);
	Pyramid = new Mesh(SHAPE::PYRAMID);
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
}

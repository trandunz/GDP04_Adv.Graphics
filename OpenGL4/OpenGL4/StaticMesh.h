// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : StaticMesh.h 
// Description : StaticMesh Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "Mesh.h"

class StaticMesh
{
public:
	/// <summary>
	/// Creates all meshes ready for use. Should be called first thing in start.
	/// </summary>
	static void Init();
	/// <summary>
	/// Cleans up all meshes and sets them nullptr. Should be called when the program is about to close.
	/// </summary>
	static void Cleanup();

	static Mesh* Quad;
	static Mesh* Cube;
	static Mesh* InvertedCube;
	static Mesh* Sphere;
	static Mesh* Hemisphere;
	static Mesh* Pyramid;
};


#pragma once
#include "Mesh.h"

class StaticMesh
{
public:
	static void Init();
	static void Cleanup();

	static Mesh* Cube;
	static Mesh* InvertedCube;
	static Mesh* Sphere;
	static Mesh* Hemisphere;
	static Mesh* Pyramid;
};


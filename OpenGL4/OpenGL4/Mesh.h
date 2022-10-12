// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Mesh.h 
// Description : Mesh Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "Helper.h"

enum class SHAPE
{
	UNASSIGNED,
	POINT,
	CUBE,
	PYRAMID,
	SPHERE,
	HEMISPHERE,
	PATCH_TRIANGLE,
	PATCH_TRIANGLE_QUAD,
	PATCH_QUAD,
	PLANE
};

class Mesh
{
public:
	/// <summary>
	/// Contruct a mesh with the given shape
	/// </summary>
	/// <param name="_shape"></param>
	Mesh(SHAPE _shape, GLenum _windingOrder = GL_CCW);
	/// <summary>
	/// Construct a 2D Mesh with the given number of sides
	/// </summary>
	/// <param name="_numberOfSides"></param>
	Mesh(unsigned int _numberOfSides, GLenum _windingOrder = GL_CCW);

	Mesh(std::string _objModel);
	/// <summary>
	/// Mesh Destructor
	/// </summary>
	~Mesh();
	/// <summary>
	/// Draws The Mesh
	/// </summary>
	void Draw();

	/// <summary>
	/// Returns a refrence too the vertices
	/// </summary>
	/// <returns></returns>
	std::vector<Vertex>& GetVertices();

	/// <summary>
	/// Returns a refrence too the indices
	/// </summary>
	/// <returns></returns>
	std::vector<unsigned>& GetIndices();

private:
	/// <summary>
	/// Populates the vertices vector with values required for the specified shape.
	/// </summary>
	/// <param name="_shape"></param>
	void CreateShapeVertices(SHAPE _shape);
	/// <summary>
	/// Populates the indices vector with values required for the specified shape.
	/// </summary>
	/// <param name="_shape"></param>
	void CreateShapeIndices(SHAPE _shape);
	/// <summary>
	/// Populates the vertices vector with values required for the specified 2D shape.
	/// </summary>
	/// <param name="_numberOfSides"></param>
	void CreatePolygonVertices(unsigned int _numberOfSides);
	/// <summary>
	/// Populates the indices vector with values required for the specified 2 shape.
	/// </summary>
	/// <param name="_numberOfSides"></param>
	void CreatePolygonIndices(unsigned int _numberOfSides);
	/// <summary>
	/// Creates the vertexArray, vertex buffer and index buffer, 
	/// populating them with the vertices and indices.
	/// Indices can be disabled by setting _ebo to false
	/// </summary>
	void CreateAndInitializeBuffersNONDSA(bool _ebo = true);
	/// <summary>
	/// Creates the vertexArray, vertex buffer and index buffer, 
	/// populating them with the vertices and indices values.
	/// Indices can be disabled by setting _ebo to false
	/// </summary>
	void CreateAndInitializeBuffersDSA(bool _ebo = true);
	/// <summary>
	/// Converts the given positional value to texture coordinate space (0-1)
	/// </summary>
	/// <param name="_position"></param>
	/// <returns></returns>
	float ToTexCoord(float& _position);
	/// <summary>
	/// Populates Vertices Array With Information For A Sphere Of Specified Fidelity
	/// </summary>
	/// <param name="_fidelity"></param>
	void GenerateSphereVertices(int _fidelity);
	/// <summary>
	/// Populates Indices Array With Information For A Sphere Of Specified Fidelity
	/// </summary>
	/// <param name="_fidelity"></param>
	void GenerateSphereIndices(int _fidelity);
	/// <summary>
	/// Populates Vertices Array With Information For A HemiSphere Of Specified Fidelity
	/// </summary>
	/// <param name="_fidelity"></param>
	void GenerateHemiSphereVertices(int _fidelity);

	std::vector<unsigned int> m_Indices{};
	std::vector<Vertex> m_Vertices{};

	SHAPE m_Shape{ 0 };

	GLuint m_VertexArrayID{ 0 };
	GLuint m_VertexBufferID{ 0 };
	GLuint m_IndexBufferID{ 0 };

	GLenum m_WindingOrder{GL_CCW};
};


// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Mesh.cpp 
// Description : Mesh Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "Mesh.h"
#include "Statics.h"
#include "Physics.h"
#include "TextureLoader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <TinyOBJ/tiny_obj_loader.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Mesh::Mesh(SHAPE _shape, GLenum _windingOrder)
{
	m_WindingOrder = _windingOrder;
	m_Shape = _shape;

	CreateShapeVertices(_shape);
	CreateShapeIndices(_shape);

	if (Statics::DSA)
		CreateAndInitializeBuffersDSA();
	else
		CreateAndInitializeBuffersNONDSA();
}

Mesh::Mesh(unsigned int _numberOfSides, GLenum _windingOrder)
{
	m_WindingOrder = _windingOrder;

	CreatePolygonVertices(_numberOfSides);
	CreatePolygonIndices(_numberOfSides);

	if (Statics::DSA)
		CreateAndInitializeBuffersDSA();
	else
		CreateAndInitializeBuffersNONDSA();
}

Mesh::Mesh(std::string _objModel)
{
	LoadModelTinyOBJ(_objModel);
	//LoadModelASSIMP(_objModel);

	if (Statics::DSA)
		CreateAndInitializeBuffersDSA(false);
	else
		CreateAndInitializeBuffersNONDSA(false);
}

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<unsigned> _indices, std::vector<Texture> _textures)
{
	m_Vertices = _vertices;
	m_Indices = _indices;
	m_AssimpTextures = _textures;

	if (Statics::DSA)
		CreateAndInitializeBuffersDSA(true);
	else
		CreateAndInitializeBuffersNONDSA(true);
}

Mesh::~Mesh()
{
	for (auto& mesh : m_AssimpMeshes)
	{
		if (mesh)
		{
			delete mesh;
			mesh = nullptr;
		}
	}
	m_AssimpMeshes.clear();
	m_Indices.clear();
	m_Vertices.clear();

	// Unbind
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// Delete
	{
		glDeleteVertexArrays(1, &m_VertexArrayID);
		glDeleteBuffers(1, &m_VertexBufferID);
		glDeleteBuffers(1, &m_IndexBufferID);
	}
}

void Mesh::Draw()
{
	if (m_AssimpMeshes.size() > 0)
	{
		for (auto& mesh : m_AssimpMeshes)
		{
			mesh->Draw();
		}
	}
	else
	{
		glBindVertexArray(m_VertexArrayID);

		if (m_Shape == SHAPE::POINT)
		{
			if (m_Indices.size() > 0)
				glDrawElements(GL_POINTS, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, nullptr);
			else
				glDrawArrays(GL_POINTS, 0, m_Vertices.size());
		}
		else if (m_Shape == SHAPE::PATCH_TRIANGLE ||
			m_Shape == SHAPE::PATCH_TRIANGLE_QUAD)
		{
			glDrawArrays(GL_PATCHES, 0, m_Vertices.size());
		}
		else if (m_Shape == SHAPE::PATCH_QUAD)
		{
			glPatchParameteri(GL_PATCH_VERTICES, 4);
			glDrawArrays(GL_PATCHES, 0, m_Vertices.size());
			glPatchParameteri(GL_PATCH_VERTICES, 3);
		}
		else if (m_Shape == SHAPE::QUAD)
		{
			if (m_Indices.size() > 0)
				glDrawElements(GL_QUADS, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, nullptr);
			else
				glDrawArrays(GL_QUADS, 0, m_Vertices.size());
		}
		else
		{
			if (m_Indices.size() > 0)
				glDrawElements(GL_TRIANGLES, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, nullptr);
			else
				glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
		}

		glBindVertexArray(0);
	}
}

std::vector<Vertex>& Mesh::GetVertices()
{
	return m_Vertices;
}

std::vector<unsigned>& Mesh::GetIndices()
{
	return m_Indices;
}

bool Mesh::RayIntersection(Ray _ray, Transform _transform)
{
	bool intersection = false;

	int index{ 0 };
	unsigned element{ 0 };
	// Loop over all indices and get points for each triangle in vertices
	for (unsigned i = 0; i < m_Indices.size() / 3; i++)
	{
		element = (m_Indices)[index];
		glm::vec4 point1{ (m_Vertices)[element].position, 1.0f };
		point1 = _transform.transform * point1;

		element = (m_Indices)[index + 1];
		glm::vec4 point2{ (m_Vertices)[element].position, 1.0f };
		point2 = _transform.transform * point2;

		element = (m_Indices)[index + 2];
		glm::vec4 point3{ (m_Vertices)[element].position, 1.0f };
		point3 = _transform.transform * point3;

		// test for intersection with triangle
		intersection = Physics::IntersectTriangle(_ray, point1, point2, point3);
		
		if (intersection)
			return true;

		index += 3;
	}

	return intersection;
}

bool Mesh::RayIntersection(Ray _ray, Transform _transform, glm::vec3& _point)
{
	bool intersection = false;

	int index{ 0 };
	unsigned element{ 0 };
	// Loop over all indices and get points for each triangle in vertices
	for (unsigned i = 0; i < m_Indices.size() / 3; i++)
	{
		element = (m_Indices)[index];
		glm::vec4 point1{ (m_Vertices)[element].position, 1.0f };
		point1 = _transform.transform * point1;

		element = (m_Indices)[index + 1];
		glm::vec4 point2{ (m_Vertices)[element].position, 1.0f };
		point2 = _transform.transform * point2;

		element = (m_Indices)[index + 2];
		glm::vec4 point3{ (m_Vertices)[element].position, 1.0f };
		point3 = _transform.transform * point3;

		// test for intersection with triangle
		intersection = Physics::IntersectTriangle(_ray, point1, point2, point3, _point);

		if (intersection)
			return true;

		index += 3;
	}

	return intersection;
}

void Mesh::CreateShapeVertices(SHAPE _shape)
{
	switch (_shape)
	{
	case SHAPE::CUBE:
	{
		// Front
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, 0.5f}, {0.0f,1.0f}, {0,0,1} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, 0.5f}, {0.0f,0.0f}, {0,0,1} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, 0.5f}, {1.0f,0.0f}, {0,0,1} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, 0.5f}, {1.0f,1.0f}, {0,0,1} });
		// Back
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, -0.5f}, {0.0f,1.0f}, {0,0,-1} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, -0.5f}, {0.0f,0.0f} , {0,0,-1} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, -0.5f}, {1.0f,0.0f}, {0,0,-1} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, -0.5f}, {1.0f,1.0f}, {0,0,-1} });
		// Right
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, 0.5f}, {0.0f,1.0f},{1,0,0} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, 0.5f}, {0.0f,0.0f},{1,0,0} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, -0.5f}, {1.0f,0.0f},{1,0,0} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, -0.5f}, {1.0f,1.0f},{1,0,0} });
		// Left
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, -0.5f}, {0.0f,1.0f},{-1,0,0} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, -0.5f}, {0.0f,0.0f},{-1,0,0} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, 0.5f}, {1.0f,0.0f},{-1,0,0} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, 0.5f}, {1.0f,1.0f},{-1,0,0} });
		// Top
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, -0.5f}, {0.0f,1.0f},{0,1,0} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, 0.5f}, {0.0f,0.0f},{0,1,0} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, 0.5f}, {1.0f,0.0f},{0,1,0} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, -0.5f}, {1.0f,1.0f},{0,1,0} });
		// Bottom
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, 0.5f}, {0.0f,1.0f},{0,-1,0} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, -0.5f}, {0.0f,0.0f},{0,-1,0} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, -0.5f}, {1.0f,0.0f},{0,-1,0} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, 0.5f}, {1.0f,1.0f},{0,-1,0} });
		break;
	}
	case SHAPE::PYRAMID:
	{
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, -0.5f}, {0.0f,1.0f} } );
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, 0.5f}, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, 0.5f}, {1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, -0.5f}, {1.0f,1.0f} });

		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, -0.5f},{0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, 0.5f}, {1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, 0.5f}, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, 0.5f},  {1.0f,0.0f} });

		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, 0.5f},  {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, -0.5f}, {1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, -0.5f}, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, -0.5f},{1.0f,0.0f} });

		// Top point / peak
		m_Vertices.emplace_back(Vertex{ {0.0f,  1.0f, 0.0f}, {0.5f,1.0f} });
		break;
	}
	case SHAPE::SPHERE:
	{
		GenerateSphereVertices(36);
		break;
	}
	case SHAPE::HEMISPHERE:
	{
		GenerateHemiSphereVertices(36);
		break;
	}
	case SHAPE::POINT:
	{
		m_Vertices.emplace_back(Vertex{});
		break;
	}
	case SHAPE::TRIANGLE:
	{
		m_Vertices.emplace_back(Vertex{ { -0.5f,  0.5f, 0.0f }, {0.0f,1.0f} });
		m_Vertices.emplace_back(Vertex{ { -0.5f,  -0.5f, 0.0f }, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ { 0.5f,  -0.5f, 0.0f }, {1.0f,0.0f} });
		break;
	}
	case SHAPE::PATCH_TRIANGLE:
	{
		m_Vertices.emplace_back(Vertex{ { -0.5f,  0.5f, 0.0f }, {0.0f,1.0f} });
		m_Vertices.emplace_back(Vertex{ { -0.5f,  -0.5f, 0.0f }, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ { 0.5f,  -0.5f, 0.0f }, {1.0f,0.0f} });
		break;
	}
	case SHAPE::PATCH_TRIANGLE_QUAD:
	{
		m_Vertices.emplace_back(Vertex{ { -0.5f,  0.5f, 0.0f }, {0.0f,1.0f} });
		m_Vertices.emplace_back(Vertex{ { -0.5f,  -0.5f, 0.0f }, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ { 0.5f,  -0.5f, 0.0f }, {1.0f,0.0f} });

		m_Vertices.emplace_back(Vertex{ { 0.5f,  -0.5f, 0.0f }, {1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ { 0.5f,  0.5f, 0.0f }, {1.0f,1.0f} });
		m_Vertices.emplace_back(Vertex{ { -0.5f,  0.5f, 0.0f }, {0.0f,1.0f} });
		break;
	}
	case SHAPE::PATCH_QUAD:
	{
		m_Vertices.emplace_back(Vertex{ { -0.5f,  -0.5f, 0.0f }, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ { 0.5f,  -0.5f, 0.0f }, {1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ { 0.5f,  0.5f, 0.0f }, {1.0f,1.0f} });
		m_Vertices.emplace_back(Vertex{ { -0.5f,  0.5f, 0.0f }, {0.0f,1.0f} });

		break;
	}
	case SHAPE::PLANE:
	{
		m_Vertices.emplace_back(Vertex{ { -0.5f,0.0f , 0.5f }, {0.0f,1.0f},{0,1,0} });
		m_Vertices.emplace_back(Vertex{ { -0.5f, 0.0f, -0.5f }, {0.0f,0.0f},{0,1,0} });
		m_Vertices.emplace_back(Vertex{ { 0.5f, 0.0f, -0.5f }, {1.0f,0.0f},{0,1,0} });

		m_Vertices.emplace_back(Vertex{ { 0.5f,0.0f , -0.5f }, {1.0f,0.0f},{0,1,0} });
		m_Vertices.emplace_back(Vertex{ { 0.5f, 0.0f, 0.5f }, {1.0f,1.0f},{0,1,0} });
		m_Vertices.emplace_back(Vertex{ { -0.5f, 0.0f, 0.5f }, {0.0f,1.0f},{0,1,0} });
		break;
	}
	case SHAPE::QUAD:
	{
		m_Vertices.emplace_back(Vertex{ { -0.5f,  -0.5f, 0.0f }, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ { 0.5f,  -0.5f, 0.0f }, {1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ { 0.5f,  0.5f, 0.0f }, {1.0f,1.0f} });
		m_Vertices.emplace_back(Vertex{ { -0.5f,  0.5f, 0.0f }, {0.0f,1.0f} });
		break;
	}
	default:
	{
		break;
	}
	}
}

void Mesh::CreateShapeIndices(SHAPE _shape)
{
	switch (_shape)
	{
	case SHAPE::CUBE:
	{
		if (m_WindingOrder == GL_CCW)
		{
			for (int i = 0; i < 6; i++)
			{
				m_Indices.emplace_back(4 * i);
				m_Indices.emplace_back((4 * i) + 1);
				m_Indices.emplace_back((4 * i) + 2);

				m_Indices.emplace_back(4 * i);
				m_Indices.emplace_back((4 * i) + 2);
				m_Indices.emplace_back((4 * i) + 3);
			}
		}
		else
		{
			for (int i = 0; i < 6; i++)
			{
				m_Indices.emplace_back(4 * i);
				m_Indices.emplace_back((4 * i) + 2);
				m_Indices.emplace_back((4 * i) + 1);

				m_Indices.emplace_back(4 * i);
				m_Indices.emplace_back((4 * i) + 3);
				m_Indices.emplace_back((4 * i) + 2);
			}
		}

		break;
	}
	case SHAPE::PYRAMID:
	{
		m_Indices.emplace_back(1);
		m_Indices.emplace_back(0);
		m_Indices.emplace_back(3);
		m_Indices.emplace_back(1);
		m_Indices.emplace_back(3);
		m_Indices.emplace_back(2);

		m_Indices.emplace_back(4);
		m_Indices.emplace_back(5);
		m_Indices.emplace_back(12);
		m_Indices.emplace_back(6);
		m_Indices.emplace_back(7);
		m_Indices.emplace_back(12);
		m_Indices.emplace_back(8);
		m_Indices.emplace_back(9);
		m_Indices.emplace_back(12);
		m_Indices.emplace_back(10);
		m_Indices.emplace_back(11);
		m_Indices.emplace_back(12);
		break;
	}
	case SHAPE::SPHERE:
	{
		GenerateSphereIndices(36);
		break;
	}
	case SHAPE::HEMISPHERE:
	{
		GenerateSphereIndices(36);
		break;
	}
	case SHAPE::POINT:
	{
		m_Indices.emplace_back(0);
		break;
	}
	case SHAPE::PATCH_TRIANGLE:
	{
		m_Indices.emplace_back(0);
		m_Indices.emplace_back(1);
		m_Indices.emplace_back(2);
		break;
	}
	case SHAPE::PATCH_TRIANGLE_QUAD:
	{
		m_Indices.emplace_back(0);
		m_Indices.emplace_back(1);
		m_Indices.emplace_back(2);
		m_Indices.emplace_back(2);
		m_Indices.emplace_back(4);
		m_Indices.emplace_back(0);
		break;
	}
	case SHAPE::PLANE:
	{
		m_Indices.emplace_back(0);
		m_Indices.emplace_back(1);
		m_Indices.emplace_back(2);
		m_Indices.emplace_back(2);
		m_Indices.emplace_back(4);
		m_Indices.emplace_back(0);
		break;
	}
	case SHAPE::QUAD:
	{
		m_Indices.emplace_back(0);
		m_Indices.emplace_back(1);
		m_Indices.emplace_back(2);
		m_Indices.emplace_back(3);
		break;
	}
	case SHAPE::TRIANGLE:
	{
		m_Indices.emplace_back(0);
		m_Indices.emplace_back(1);
		m_Indices.emplace_back(2);
		break;
	}
	default:
	{
		break;
	}
	}
}

void Mesh::CreatePolygonVertices(unsigned int _numberOfSides)
{
	// If Its Got 4 Sides, Create Generic Quad
	if (_numberOfSides == 4)
	{
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, 0.0f}, {0.0f,1.0f} }); // Top Left
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, 0.0f}, {0.0f,0.0f} }); // Bottom Left
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, 0.0f}, {1.0f,0.0f} }); // Bottom Right
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, 0.0f}, {1.0f,1.0f} }); // Top Right
		return;
	}

	float angle = 0.0f, increment = (glm::two_pi<float>() / _numberOfSides);

	// Centre
	m_Vertices.emplace_back(Vertex{ {0.0f,  0.0f, 0.0f}, {0.5f,0.5f} });

	// Fan Around Centre
	float xPos, yPos;
	for (unsigned i = 0; i < _numberOfSides; i++)
	{
		xPos = cos(angle);
		yPos = sin(angle);
		m_Vertices.emplace_back(Vertex{ {xPos, yPos, 0 },{ToTexCoord(xPos),ToTexCoord(yPos)} });
		angle += increment;
	}
}

void Mesh::CreatePolygonIndices(unsigned int _numberOfSides)
{
	// If Its Got 4 Sides, Create Generic Quad Indices
	if (_numberOfSides == 4)
	{
		m_Indices.emplace_back(0);	// Top Left
		m_Indices.emplace_back(1);	// Bottom Left
		m_Indices.emplace_back(2);	// Bottom Right
		m_Indices.emplace_back(0);	// Top Left
		m_Indices.emplace_back(2);	// Bottom Right
		m_Indices.emplace_back(3);	// Top Right
		return;
	}

	// Generate Generalised Regular Polygon Indices
	for (unsigned i = 0; i < _numberOfSides; i++)
	{
		// Centre
		m_Indices.push_back(0);

		// Back At Beginning?
		if (i + 2 > _numberOfSides)
		{
			m_Indices.push_back(i + 1);
			m_Indices.push_back(1);
		}
		else
		{
			m_Indices.push_back(i + 1);
			m_Indices.push_back(i + 2);
		}
	}
}

void Mesh::CreateAndInitializeBuffersNONDSA(bool _ebo)
{
	// Vertex Array
	glGenVertexArrays(1, &m_VertexArrayID);
	glBindVertexArray(m_VertexArrayID);

	// Vertex Buffer
	glGenBuffers(1, &m_VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

	// Index Buffer
	if (m_Indices.size() > 0)
	{
		glGenBuffers(1, &m_IndexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);
	}

	// Layouts
	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// TexCoords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoords)));
	// Normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normals)));
	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::CreateAndInitializeBuffersDSA(bool _ebo)
{
	// Vertex Array
	glCreateVertexArrays(1, &m_VertexArrayID);

	// Vertex Buffer
	glCreateBuffers(1, &m_VertexBufferID);
	glNamedBufferData(m_VertexBufferID, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

	// Layouts
	// Position
	glEnableVertexArrayAttrib(m_VertexArrayID, 0);
	glVertexArrayAttribBinding(m_VertexArrayID, 0, 0);
	glVertexArrayAttribFormat(m_VertexArrayID, 0, 3, GL_FLOAT, GL_FALSE, 0);
	// TexCoords
	glEnableVertexArrayAttrib(m_VertexArrayID, 1);
	glVertexArrayAttribBinding(m_VertexArrayID, 1, 0);
	glVertexArrayAttribFormat(m_VertexArrayID, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoords));
	// Normals
	glEnableVertexArrayAttrib(m_VertexArrayID, 2);
	glVertexArrayAttribBinding(m_VertexArrayID, 2, 0);
	glVertexArrayAttribFormat(m_VertexArrayID, 2, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normals));

	// Attach Vertex Buffer To vertex array
	glVertexArrayVertexBuffer(m_VertexArrayID, 0, m_VertexBufferID, 0, sizeof(Vertex));

	// Index Buffer
	if (_ebo)
	{
		glCreateBuffers(1, &m_IndexBufferID);
		glNamedBufferData(m_IndexBufferID, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

		// Attach index buffer to vertex array
		glVertexArrayElementBuffer(m_VertexArrayID, m_IndexBufferID);
	}
}

float Mesh::ToTexCoord(float& _position)
{
	return (_position + 1) * 0.5f;
}

void Mesh::GenerateSphereVertices(int _fidelity)
{
	// Angles to keep track of the sphere points 
	float Phi = 0.0f;
	float Theta = 0.0f;

	// Cycle through x axis by increments of 1 / fidelity level
	for (int i = 0; i < _fidelity; i++)
	{
		// Starting angle of 0 for each y axis ring
		Theta = 0.0f;

		// Cycle through y axis by increments of 1 / fidelity level
		for (int j = 0; j < _fidelity; j++)
		{
			// Calculate new vertex positions based on 
			// current y axis angle and x axis angle
			float x = cos(Phi) * sin(Theta);
			float y = cos(Theta);
			float z = sin(Phi) * sin(Theta);

			// Add the new vertex point to the vertices vector
			m_Vertices.emplace_back(Vertex{
				{ x * 0.5f, y * 0.5f, z * 0.5f }, // Position
				{ 1 - (float)i / (_fidelity - 1), 1 - ((float)j / (_fidelity - 1)) }, // Texture coords
				{ x,y,z } // Normals
				});

			// update y axis angle by increments of PI / fidelity level
			// As the sphere is built ring by ring, 
			// the angle is only needed to do half the circumferance therefore using just PI
			Theta += (glm::pi<float>() / ((float)_fidelity - 1.0f));
		}

		// x and z axis angle is updated by increments of 2Pi / fidelity level
		// Angle uses 2*PI to get the full circumference as this layer is built as a full ring
		Phi += (glm::two_pi<float>()) / ((float)_fidelity - 1.0f);
	}
}

void Mesh::GenerateSphereIndices(int _fidelity)
{
	for (int i = 0; i < _fidelity; i++)
	{
		for (int j = 0; j < _fidelity; j++)
		{
			if (m_WindingOrder == GL_CCW)
			{
				// First triangle of the quad
				m_Indices.emplace_back((((i + 1) % _fidelity) * _fidelity) + ((j + 1) % _fidelity));
				m_Indices.emplace_back((i * _fidelity) + (j));
				m_Indices.emplace_back((((i + 1) % _fidelity) * _fidelity) + (j));

				// Second triangle of the quad
				m_Indices.emplace_back((i * _fidelity) + ((j + 1) % _fidelity));
				m_Indices.emplace_back((i * _fidelity) + (j));
				m_Indices.emplace_back((((i + 1) % _fidelity) * _fidelity) + ((j + 1) % _fidelity));
			}
			else
			{
				// First triangle of the quad
				m_Indices.emplace_back((((i + 1) % _fidelity) * _fidelity) + ((j + 1) % _fidelity));
				m_Indices.emplace_back((((i + 1) % _fidelity) * _fidelity) + (j));
				m_Indices.emplace_back((i * _fidelity) + (j));

				// Second triangle of the quad
				m_Indices.emplace_back((i * _fidelity) + ((j + 1) % _fidelity));
				m_Indices.emplace_back((((i + 1) % _fidelity) * _fidelity) + ((j + 1) % _fidelity));
				m_Indices.emplace_back((i * _fidelity) + (j));
			}
		}
	}
}

void Mesh::GenerateHemiSphereVertices(int _fidelity)
{
	// Angles to keep track of the sphere points 
	float Phi = 0.0f;
	float Theta = 0.0f;

	// Cycle through x axis by increments of 1 / fidelity level
	for (int i = 0; i < _fidelity; i++)
	{
		// Starting angle of 0 for each y axis ring
		Theta = 0.0f;

		// Cycle through y axis by increments of 1 / fidelity level
		for (int j = 0; j < _fidelity; j++)
		{
			// Calculate new vertex positions based on 
			// current y axis angle and x axis angle
			float x = cos(Phi) * sin(Theta);
			float y = cos(Theta);
			float z = sin(Phi) * sin(Theta);

			// Add the new vertex point to the vertices vector
			m_Vertices.emplace_back(Vertex{
				{ x * 0.5f, y * 0.5f, z * 0.5f }, // Position
				{ 1 - (float)i / (_fidelity - 1), 1 - ((float)j / (_fidelity - 1)) }, // Texture coords
				{ x,y,z } // Normals
				});

			// update y axis angle by increments of PI / fidelity level
			// As the sphere is built ring by ring, 
			// the angle is only needed to do half the circumferance therefore using just PI
			Theta += (glm::pi<float>() / ((float)_fidelity - 1.0f));
		}

		// x and z axis angle is updated by increments of 2Pi / fidelity level
		// Angle uses 2*PI to get the full circumference as this layer is built as a full ring
		Phi += (glm::pi<float>()) / ((float)_fidelity - 1.0f);
	}
}

void Mesh::LoadModelTinyOBJ(std::string _path)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string error;
	_path = "Resources/Models/" + _path;
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &error, _path.c_str()))
	{
		throw std::runtime_error(error);
	}

	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			Vertex vertice{};
			if (index.vertex_index >= 0)
			{
				vertice.position =
				{
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};
			}
			if (index.normal_index >= 0)
			{
				vertice.normals =
				{
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2]
				};
			}
			if (index.texcoord_index >= 0)
			{
				vertice.texCoords =
				{
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1]
				};
			}
			m_Vertices.push_back(vertice);
		}
	}
}

void Mesh::LoadModelASSIMP(std::string _path)
{
	//Assimp::Importer importer;
	//const aiScene* scene = importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs);
	//
	//if (!scene)
	//	Print("Failed to load model " + _path);
	//else if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	//	Print("Failed to load model " + _path);
	//else
	//{
	//	ProcessNode(scene->mRootNode, scene);
	//}
}

void Mesh::ProcessNode(aiNode* _node, const aiScene* _scene)
{
	//for (unsigned i = 0; i < _node->mNumMeshes; i++)
	//{
	//	aiMesh* mesh = _scene->mMeshes[_node->mMeshes[i]];
	//	m_AssimpMeshes.push_back(ProcessMesh(mesh, _scene));
	//}
	//
	//for (unsigned i = 0; i < _node->mNumChildren; i++)
	//{
	//	ProcessNode(_node->mChildren[i], _scene);
	//}
}

Mesh* Mesh::ProcessMesh(aiMesh* _mesh, const aiScene* _scene)
{
	std::vector<Vertex> vertices{};
	std::vector<unsigned> indices{};
	std::vector<Texture> textures{};
	//
	//// vertices
	//for (unsigned i = 0; i < _mesh->mNumVertices; i++)
	//{
	//	Vertex vertex{};
	//	if (_mesh->mVertices)
	//	{
	//		vertex.position = { _mesh->mVertices[i].x, _mesh->mVertices[i].y, _mesh->mVertices[i].z };
	//	}
	//	if (_mesh->mNormals)
	//	{
	//		vertex.normals = { _mesh->mNormals[i].x, _mesh->mNormals[i].y, _mesh->mNormals[i].z };
	//	}
	//	if (_mesh->mTextureCoords[0])
	//	{
	//		vertex.texCoords = { _mesh->mTextureCoords[0][i].x, _mesh->mTextureCoords[0][i].y };
	//	}
	//
	//	vertices.push_back(vertex);
	//}
	//
	//// indices
	//for (unsigned i = 0; i < _mesh->mNumFaces; i++)
	//{
	//	aiFace face = _mesh->mFaces[i];
	//	for (unsigned j = 0; j < face.mNumIndices; j++)
	//	{
	//		indices.push_back(face.mIndices[j]);
	//	}
	//}
	//
	//// material
	//if (_mesh->mMaterialIndex >= 0)
	//{
	//	aiMaterial* material = _scene->mMaterials[_mesh->mMaterialIndex];
	//
	//	// diffuse
	//	std::vector<Texture> diffuseMaps = LoadTextures(material, aiTextureType_DIFFUSE);
	//	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	//
	//	// specular
	//	std::vector<Texture> specularMaps = LoadTextures(material, aiTextureType_SPECULAR);
	//	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	//}
	//
	return new Mesh(vertices, indices, textures);
}

std::vector<Texture> Mesh::LoadTextures(aiMaterial* _mat, aiTextureType _type)
{
	std::vector<Texture> textures;
	//for (unsigned i = 0; i < _mat->GetTextureCount(_type); i++)
	//{
	//	aiString string{};
	//	_mat->GetTexture(_type, i, &string);
	//
	//	bool skipTexture{ false };
	//	for (unsigned j = 0; j < m_AssimpTextures.size(); j++)
	//	{
	//		if (std::strcmp(m_AssimpTextures[j].FilePath.data(), string.C_Str()) == 0)
	//		{
	//			textures.push_back(m_AssimpTextures[j]);
	//			skipTexture = true;
	//			break;
	//		}
	//	}
	//
	//	if (!skipTexture)
	//	{
	//		Texture texture = TextureLoader::LoadTexture(string.C_Str());
	//		textures.push_back(texture);
	//		m_AssimpTextures.push_back(texture);
	//	}
	//}
	//
	return textures;
}

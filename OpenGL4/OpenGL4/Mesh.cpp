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

#define TINYOBJLOADER_IMPLEMENTATION
#include <TinyOBJ/tiny_obj_loader.h>

Mesh::Mesh(SHAPE _shape, GLenum _windingOrder)
{
	m_WindingOrder = _windingOrder;

	CreateShapeVertices(_shape);
	CreateShapeIndices(_shape);
	CreateAndInitializeBuffers();
}

Mesh::Mesh(unsigned int _numberOfSides, GLenum _windingOrder)
{
	m_WindingOrder = _windingOrder;

	CreatePolygonVertices(_numberOfSides);
	CreatePolygonIndices(_numberOfSides);
	CreateAndInitializeBuffers();
}

Mesh::Mesh(std::string _objModel)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string error;
	_objModel = "Resources/Models/" + _objModel;
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &error, _objModel.c_str()))
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

	CreateAndInitializeBuffers(false);
}

Mesh::~Mesh()
{
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
	glBindVertexArray(m_VertexArrayID);

	if (m_Indices.size() > 0)
		glDrawElements(GL_TRIANGLES, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, nullptr);
	else
		glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());

	glBindVertexArray(0);
}

std::vector<Vertex>& Mesh::GetVertices()
{
	return m_Vertices;
}

std::vector<unsigned>& Mesh::GetIndices()
{
	return m_Indices;
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

void Mesh::CreateAndInitializeBuffers(bool _ebo)
{
	// Vertex Array
	glGenVertexArrays(1, &m_VertexArrayID);
	glBindVertexArray(m_VertexArrayID);

	// Vertex Buffer
	glGenBuffers(1, &m_VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

	// Index Buffer
	if (_ebo)
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

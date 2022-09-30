// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Terrain.cpp 
// Description : Terrain Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "Terrain.h"
#include "Noise.h"
#include "Statics.h"


Terrain::Terrain(std::string _heightMap, std::string _fileExtension)
{
	m_ShaderID = ShaderLoader::CreateShader("Terrain.vert","TerrainTextures.frag");

	if (_fileExtension == ".RAW")
		LoadRAWHeightmap(_heightMap + _fileExtension);
	else
		LoadImageHeightmap(_heightMap + _fileExtension);

	Smooth();
	GenerateVertices();
	GenerateIndices();
	CreateAndInitializeBuffers(Statics::DSA);
}

Terrain::Terrain()
{
	m_ShaderID = ShaderLoader::CreateShader("Terrain.vert", "TerrainTextures.frag");

	GenerateRandomHeightmap();
	Smooth();
	GenerateVertices();
	GenerateIndices();
	CreateAndInitializeBuffers(Statics::DSA);
}

Terrain::~Terrain()
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

void Terrain::Draw()
{
	glUseProgram(m_ShaderID);

	// Projection * View * Model Matrix
	ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "PVMMatrix", Statics::SceneCamera.GetPVMatrix() * m_Transform.transform);

	// Set Model Matrix
	ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "ModelMatrix", m_Transform.transform);

	SetBlinnFong3DUniforms();

	// Set fog uniforms
	ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Foggy", Statics::Foggy);
	if (Statics::Foggy)
	{
		ShaderLoader::SetUniform1f(std::move(m_ShaderID), "FogStart", 5.0f);
		ShaderLoader::SetUniform1f(std::move(m_ShaderID), "FogDepth", 10.0f);
		ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "CameraPos", Statics::SceneCamera.GetPosition());
		ShaderLoader::SetUniform4fv(std::move(m_ShaderID), "FogColor", { 0.5f, 0.5f, 0.5f, 1.0f });
	}

	// Set different levels that textures are used
	// texture0 = 0.0f -> 8.0f
	// e.t.c
	ShaderLoader::SetUniform1f(std::move(m_ShaderID), "Texture0Height", 0.0f);
	ShaderLoader::SetUniform1f(std::move(m_ShaderID), "Texture1Height", 8.0f);
	ShaderLoader::SetUniform1f(std::move(m_ShaderID), "Texture2Height", 30.0f);
	ShaderLoader::SetUniform1f(std::move(m_ShaderID), "Texture3Height", 150.0f);

	glBindVertexArray(m_VertexArrayID);

	glDrawElements(GL_TRIANGLES, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
	
	glUseProgram(0);
}

void Terrain::SetTexture(Texture _texture)
{
	m_Texture = _texture;
}

void Terrain::SetTranslation(glm::vec3 _newPosition)
{
	m_Transform.translation = _newPosition;
	UpdateModelValueOfTransform(m_Transform);
}

void Terrain::Translate(glm::vec3 _translation)
{
	m_Transform.translation += _translation;
	UpdateModelValueOfTransform(m_Transform);
}

void Terrain::SetRotation(glm::vec3 _axis, float _degrees)
{
	m_Transform.rotation_axis = _axis;
	m_Transform.rotation_value = glm::radians(_degrees);
	UpdateModelValueOfTransform(m_Transform);
}

void Terrain::Rotate(glm::vec3 _axis, float _degrees)
{
	m_Transform.rotation_axis = _axis;
	m_Transform.rotation_value += glm::radians(_degrees);
	UpdateModelValueOfTransform(m_Transform);
}

void Terrain::SetScale(glm::vec3 _newScale)
{
	m_Transform.scale = _newScale;
	UpdateModelValueOfTransform(m_Transform);
}

void Terrain::Scale(glm::vec3 _scaleFactor)
{
	m_Transform.scale *= _scaleFactor;
	UpdateModelValueOfTransform(m_Transform);
}

void Terrain::SetActiveTextures(std::vector<Texture> _textures)
{
	m_ActiveTextures = _textures;
}

void Terrain::CreateAndInitializeBuffers(bool _dsa)
{
	if (_dsa)
	{
		// Vertex Array
		glCreateVertexArrays(1, &m_VertexArrayID);

		// Vertex Buffer
		glCreateBuffers(1, &m_VertexBufferID);
		glNamedBufferData(m_VertexBufferID, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

		// Index Buffer
		glCreateBuffers(1, &m_IndexBufferID);
		glNamedBufferData(m_IndexBufferID, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

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
		// Attach Index buffer to vertex array
		glVertexArrayElementBuffer(m_VertexArrayID, m_IndexBufferID);
	}
	else
	{
		// Vertex Array
		glGenVertexArrays(1, &m_VertexArrayID);
		glBindVertexArray(m_VertexArrayID);

		// Vertex Buffer
		glGenBuffers(1, &m_VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

		// Index Buffer
		glGenBuffers(1, &m_IndexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

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
}

void Terrain::GenerateVertices()
{
	// Clear the vertices and reserve 513 * 513 spaces
	m_Vertices.clear();
	m_Vertices.resize(513 * 513);

	float halfWidth = (512) * 0.5f;
	float halfDepth = (512) * 0.5f;

	// Get Vertex step for tex coords
	float du = 1.0f / 512;
	float dv = 1.0f / 512;
	for (unsigned i = 0; i < 513; ++i)
	{
		float z = halfDepth - i;
		for (unsigned j = 0; j < 513; ++j)
		{
			float x = -halfWidth + j;

			float y = m_HeightMap[i * 513 + j];
			m_Vertices[i * 513 + j].position = { x, y, -z };
			// Normal is up too start
			m_Vertices[i * 513 + j].normals = { 0.0f, 1.0f, 0.0f };

			// Stretch texture over grid.
			m_Vertices[i * 513 + j].texCoords.x = j * du;
			m_Vertices[i * 513 + j].texCoords.y = i * dv;
		}
	}

	// Estimate normals for interior nodes using central difference.
	float invTwoDX = 1.0f / 2.0f;
	float invTwoDZ = 1.0f / 2.0f;
	for (unsigned i = 2; i < 512; ++i)
	{
		for (unsigned j = 2; j < 512; ++j)
		{
			float t = m_HeightMap[(i - 1) * 513 + j];
			float b = m_HeightMap[(i + 1) * 513 + j];
			float l = m_HeightMap[i * 513 + j - 1];
			float r = m_HeightMap[i * 513 + j + 1];

			glm::vec3 tanZ(0.0f, (t - b) * invTwoDZ, 1.0f);
			glm::vec3 tanX(1.0f, (r - l) * invTwoDX, 0.0f);

			
			glm::vec3 normal = glm::normalize(glm::cross(tanZ, tanX));

			m_Vertices[i * 513 + j].normals = normal;
		}
	}
}

void Terrain::GenerateIndices()
{
	// Clear the indices and reserve 512 * 512 spaces
	m_Indices.clear();
	m_Indices.resize((512) * (512) * 2 * 3);
	// Iterate over each quad and make indices.
	int k = 0;
	for (unsigned i = 0; i < 512; ++i)
	{
		for (unsigned j = 0; j < 512; ++j)
		{
			// First triangle
			m_Indices[k] = i * 513 + j;
			m_Indices[k + 1] = i * 513 + j + 1;
			m_Indices[k + 2] = (i + 1) * 513 + j;

			// Second triangle
			m_Indices[k + 3] = (i + 1) * 513 + j;
			m_Indices[k + 4] = i * 513 + j + 1;
			m_Indices[k + 5] = (i + 1) * 513 + j + 1;

			k += 6; // next quad
		}
	}
}

void Terrain::LoadRAWHeightmap(std::string _fileName)
{
	// A height for each vertex
	std::vector<unsigned char> in(513 * 513);

	// Open the file.
	std::ifstream inFile;
	inFile.open("Resources/Textures/Heightmaps/" + _fileName, std::ios_base::binary);

	if (inFile)
	{
		// Read the RAW bytes.
		inFile.read((char*)&in[0], (std::streamsize)(513 * 513));

		// Done with file.
		inFile.close();
	}

	// Copy the array data into heightmap vector
	m_HeightMap.resize(513 * 513, 0);
	for (unsigned i = 0; i < 513 * 513; ++i)
	{
		m_HeightMap[i] = (float)in[i];
	}
}

void Terrain::LoadImageHeightmap(std::string _fileName)
{
	// Get the array of ppixel data
	std::vector<unsigned char> in = TextureLoader::LoadHeightMap(std::move(_fileName));
	m_HeightMap.resize(in.size(), 0);
	for (unsigned i = 0; i < in.size(); ++i)
	{
		m_HeightMap[i] = (float)in[i];
	}
}

void Terrain::GenerateRandomHeightmap()
{
	// Populate the heightmap array with noise
	int index = 0;
	m_HeightMap.resize(513 * 513, 0);
	for (int i = 0; i < 513; i++)
	{
		for (int j = 0; j < 513; j++)
		{
			m_HeightMap[index++] = 128 * Noise::TotalNoisePerPoint(j, i) + 128;
		}
	}
}

void Terrain::Smooth()
{
	std::vector<float> dest(m_HeightMap.size());

	for (unsigned i = 0; i < 513; ++i)
	{
		for (unsigned j = 0; j < 513; ++j)
		{
			dest[i * 513 + j] = Average(i, j);
		}
	}

	// Replace the old heightmap with the smoothed one
	m_HeightMap = dest;
}

float Terrain::Average(unsigned _i, unsigned _j)
{
	float avg = 0.0f;
	float num = 0.0f;

	for (unsigned m = _i - 1; m <= _i + 1; ++m)
	{
		for (unsigned n = _j; n <= _j + 1; ++n)
		{
			if (InBounds(m, n))
			{
				avg += m_HeightMap[m * 513 + n];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}

bool Terrain::InBounds(unsigned _i, unsigned _j)
{
	return _i >= 0 && _i < 513 && _j >= 0 && _j < 513;
}

void Terrain::SetBlinnFong3DUniforms()
{
	// Apply Texture
	for(size_t i = 0 ; i < m_ActiveTextures.size(); i++)
	{
		ShaderLoader::SetUniform1i(std::move(m_ShaderID), "TextureCount", m_ActiveTextures.size());
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_ActiveTextures[i].ID);
		ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Texture" + std::to_string(i), i);
	}

	// Set Global Ambient Colour And Strength
	ShaderLoader::SetUniform1f(std::move(m_ShaderID), "AmbientStrength", 0.15f);
	ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "AmbientColor", { 1.0f,1.0f,1.0f });

	// Set Shininess
	ShaderLoader::SetUniform1f(std::move(m_ShaderID), "Shininess", 32.0f * 5);

	// Set Camera Position
	ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "CameraPos", Statics::SceneCamera.GetPosition());

	// Set Point Light Uniforms From Light Manager
	std::vector<PointLight>& pointLights = LightManager::GetInstance().GetPointLights();
	ShaderLoader::SetUniform1i(std::move(m_ShaderID), "NumberOfPointLights", (int)pointLights.size());
	for (unsigned i = 0; i < pointLights.size(); i++)
	{
		ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "PointLights[" + std::to_string(i) + "].Position", pointLights[i].Position);
		ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "PointLights[" + std::to_string(i) + "].Color", pointLights[i].Color);
		ShaderLoader::SetUniform1f(std::move(m_ShaderID), "PointLights[" + std::to_string(i) + "].SpecularStrength", pointLights[i].SpecularStrength);
		ShaderLoader::SetUniform1f(std::move(m_ShaderID), "PointLights[" + std::to_string(i) + "].AttenuationLinear", pointLights[i].AttenuationLinear);
		ShaderLoader::SetUniform1f(std::move(m_ShaderID), "PointLights[" + std::to_string(i) + "].AttenuationExponent", pointLights[i].AttenuationExponent);
	}

	// Set Directional Light Uniforms From Light Manager
	std::vector<DirectionalLight>& directionalLights = LightManager::GetInstance().GetDirectionalLights();
	ShaderLoader::SetUniform1i(std::move(m_ShaderID), "NumberOfDirectionalLights", (int)directionalLights.size());
	for (unsigned i = 0; i < directionalLights.size(); i++)
	{
		ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "DirectionalLights[" + std::to_string(i) + "].Direction", directionalLights[i].Direction);
		ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "DirectionalLights[" + std::to_string(i) + "].Color", directionalLights[i].Color);
		ShaderLoader::SetUniform1f(std::move(m_ShaderID), "DirectionalLights[" + std::to_string(i) + "].SpecularStrength", directionalLights[i].SpecularStrength);
	}

	// Set Spotlight Uniforms From Light Manager
	std::vector<SpotLight>& spotLights = LightManager::GetInstance().GetSpotLights();
	ShaderLoader::SetUniform1i(std::move(m_ShaderID), "NumberOfSpotLights", (int)spotLights.size());
	for (unsigned i = 0; i < spotLights.size(); i++)
	{
		// If the spotlight is attached to the camera, Set Uniforms Accordingly
		if (spotLights[i].IsAttachedToCamera)
		{
			ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].Position", Statics::SceneCamera.GetPosition());
			ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].Direction", Statics::SceneCamera.GetFront());
		}
		// Else Apply Assigned Starting Positon And Direction
		else
		{
			ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].Position", spotLights[i].Position);
			ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].Direction", spotLights[i].Direction);
		}
		ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].Color", spotLights[i].Color);
		ShaderLoader::SetUniform1f(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].SpecularStrength", spotLights[i].SpecularStrength);
		ShaderLoader::SetUniform1f(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].AttenuationLinear", spotLights[i].AttenuationLinear);
		ShaderLoader::SetUniform1f(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].AttenuationExponent", spotLights[i].AttenuationExponent);
		ShaderLoader::SetUniform1f(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].Cutoff", glm::cos(glm::radians(spotLights[i].Cutoff)));
		ShaderLoader::SetUniform1f(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].OuterCutoff", glm::cos(glm::radians(spotLights[i].OuterCutoff)));
	}
}
